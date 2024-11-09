#include "RHI/VulkanRHI/VulkanTypes/VulkanTexture.h"

namespace Core
{
	void VulkanTexture::CreateTextureImage()
	{
		// Creates transfer buffers
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Creates buffer that can transfer memory from CPU to GPU
		CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Map the data into the buffer
		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, texture, static_cast<size_t>(imageSize));
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Free the texture on the CPU
		stbi_image_free(texture);

		// Creates the image object
		CreateImage(static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);

		// Transition the image layout a first time
		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy the texture in the good buffer
		CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight));

		// Transition the image layout a second time
		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Destroys the transfer buffer
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
	}

	void VulkanTexture::CreateTextureImageView()
	{
		// Uses the create image view function
		m_TextureImageView = CreateImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void VulkanTexture::CreateTextureSampler()
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
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

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
		VkResult result = vkCreateSampler(m_LogicalDevice, &samplerInfo, nullptr, &m_TextureSampler);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture sampler, Error Code: %d", result);
			return;
		}
	}

	RHI_RESULT VulkanTexture::DestroyTexture()
	{
		vkDestroySampler(m_LogicalDevice, m_TextureSampler, nullptr);

		// Other destroy

		return RHI_RESULT();
	}
}