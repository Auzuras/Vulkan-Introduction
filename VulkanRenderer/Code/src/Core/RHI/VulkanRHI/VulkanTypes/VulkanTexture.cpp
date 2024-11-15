#include "RHI/VulkanRHI/VulkanTypes/VulkanTexture.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanBuffer.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandAllocator.h"
#include "Renderer.h"

namespace Core
{
	void VulkanTexture::CreateTextureImage(VulkanDevice* _Device, unsigned char* _TextureData, int _Width, int _Height)
	{
		VkDeviceSize imageSize = _Width * _Height * 4;

		// Creates transfer buffers
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		VulkanBuffer buffer;

		// Creates buffer that can transfer memory from CPU to GPU
		buffer.CreateBuffer(_Device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Map the data into the buffer
		void* data;
		vkMapMemory(_Device->GetLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, _TextureData, static_cast<size_t>(imageSize));
		vkUnmapMemory(_Device->GetLogicalDevice(), stagingBufferMemory);

		// Creates the image object
		m_TextureImage.CreateImage(_Device, static_cast<uint32_t>(_Width), static_cast<uint32_t>(_Height), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage.GetImage(), m_TextureImageMemory);

		// Transition the image layout a first time
		m_TextureImage.TransitionImageLayout(_Device, m_TextureImage.GetImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy the texture in the good buffer
		VulkanBuffer::CopyBufferToImage(_Device, Core::Renderer::GetCommandAllocator()->CastToVulkan(), stagingBuffer, m_TextureImage, static_cast<uint32_t>(_Width), static_cast<uint32_t>(_Height));

		// Transition the image layout a second time
		m_TextureImage.TransitionImageLayout(_Device, m_TextureImage.GetImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Destroys the transfer buffer
		vkDestroyBuffer(_Device->GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(_Device->GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void VulkanTexture::CreateTextureImageView(VulkanDevice* _Device)
	{
		// Uses the create image view function
		m_TextureImageView.CreateImageView(_Device->GetLogicalDevice(), m_TextureImage.GetImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void VulkanTexture::CreateTextureSampler(VulkanDevice* _Device)
	{
		// Sampler infos
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		// Same as OpenGL
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16.0f;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(_Device->GetPhysicalDevice(), &properties);

		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		// VK_FALSE = coords are between 0 and width or height and VK_TRUE coords are between 0 and 1
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		// Mip maps infos
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		// Creates the sampler
		VkResult result = vkCreateSampler(_Device->GetLogicalDevice(), &samplerInfo, nullptr, &m_TextureSampler);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture sampler, Error Code: %d", result);
			return;
		}
	}

	RHI_RESULT VulkanTexture::CreateTexture(IDevice* _Device, unsigned char* _TextureData, int _Width, int _Height)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		CreateTextureImage(&device, _TextureData, _Width, _Height);
		CreateTextureImageView(&device);
		CreateTextureSampler(&device);

		return RHI_SUCCESS;
	}

	RHI_RESULT VulkanTexture::DestroyTexture(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroySampler(device.GetLogicalDevice(), m_TextureSampler, nullptr);
		vkDestroyImageView(device.GetLogicalDevice(), m_TextureImageView.GetType(), nullptr);

		vkDestroyImage(device.GetLogicalDevice(), m_TextureImage.GetImage(), nullptr);
		vkFreeMemory(device.GetLogicalDevice(), m_TextureImageMemory, nullptr);

		return RHI_SUCCESS;
	}
}