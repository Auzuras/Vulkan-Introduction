#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptorAllocator.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanBuffer.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanTexture.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptorLayout.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptor.h"

namespace Core
{

	VulkanDescriptorAllocator::~VulkanDescriptorAllocator()
	{}

	const RHI_RESULT VulkanDescriptorAllocator::CreateDescriptorAllocator(IDevice* _Device, ISwapChain* _Swapchain)
	{
		VulkanDevice device = *_Device->CastToVulkan();
		VulkanSwapChain swapchain = *_Swapchain->CastToVulkan();

		// Describes the pool size
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		// UBO
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchain.GetSwapchainImagesNbr() * 5);
		// Sampler
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchain.GetSwapchainImagesNbr() * 5);

		// Pool infos
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		// All Descriptors size
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapchain.GetSwapchainImagesNbr() * 5);

		// Creates a descriptor pool
		VkResult result = vkCreateDescriptorPool(device.GetLogicalDevice(), &poolInfo, nullptr, &m_DescriptorPool);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create descriptor pool, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	const RHI_RESULT VulkanDescriptorAllocator::DestroyDescriptorAllocator(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroyDescriptorPool(device.GetLogicalDevice(), m_DescriptorPool, nullptr);

		return RHI_SUCCESS;
	}

	std::vector<IDescriptor*> VulkanDescriptorAllocator::CreateTextureDescriptor(IDevice* _Device, int _DescriptorNbr, ITexture* _Texture, IDescriptorLayout* _Layout)
	{
		VulkanDevice device = *_Device->CastToVulkan();
		VulkanDescriptorLayout layout = *_Layout->CastToVulkan();

		std::vector<VkDescriptorSetLayout> layouts(_DescriptorNbr, layout.GetType());

		// Allocation infos
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		// Size and data of all the Descriptor set layout
		allocInfo.descriptorSetCount = static_cast<uint32_t>(_DescriptorNbr);
		allocInfo.pSetLayouts = layouts.data();

		std::vector<VkDescriptorSet> descriptorSets(_DescriptorNbr);

		// Allocates memory for all descriptor sets
		VkResult result = vkAllocateDescriptorSets(device.GetLogicalDevice(), &allocInfo, descriptorSets.data());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate descriptor sets, Error Code: %d", result);
			return std::vector<IDescriptor*>();
		}

		std::vector<IDescriptor*> descriptors(_DescriptorNbr);

		for (size_t i = 0; i < _DescriptorNbr; ++i)
		{
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = _Texture->CastToVulkan()->GetImageView().GetType();
			imageInfo.sampler = _Texture->CastToVulkan()->GetSampler();

			VkWriteDescriptorSet descriptorWrites{};

			descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites.pNext = nullptr;
			descriptorWrites.dstSet = descriptorSets[i];
			descriptorWrites.dstBinding = 0;
			descriptorWrites.dstArrayElement = 0;
			descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites.descriptorCount = 1;
			descriptorWrites.pBufferInfo = nullptr;
			descriptorWrites.pImageInfo = &imageInfo;
			descriptorWrites.pTexelBufferView = nullptr;

			vkUpdateDescriptorSets(device.GetLogicalDevice(), 1, &descriptorWrites, 0, nullptr);

			descriptors[i] = new VulkanDescriptor(descriptorSets[i]);
		}

		return descriptors;
	}

	std::vector<IDescriptor*> VulkanDescriptorAllocator::CreateUBODescriptor(IDevice* _Device, int _DescriptorNbr, std::vector<IBuffer*> _Buffer, size_t _BufferSize, IDescriptorLayout* _Layout)
	{
		VulkanDevice device = *_Device->CastToVulkan();
		VulkanDescriptorLayout layout = *_Layout->CastToVulkan();

		std::vector<VkDescriptorSetLayout> layouts(_DescriptorNbr, layout.GetType());

		// Allocation infos
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		// Size and data of all the Descriptor set layout
		allocInfo.descriptorSetCount = static_cast<uint32_t>(_DescriptorNbr);
		allocInfo.pSetLayouts = layouts.data();

		std::vector<VkDescriptorSet> descriptorSets(_DescriptorNbr);

		// Allocates memory for all descriptor sets
		VkResult result = vkAllocateDescriptorSets(device.GetLogicalDevice(), &allocInfo, descriptorSets.data());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate descriptor sets, Error Code: %d", result);
			return std::vector<IDescriptor*>();
		}

		std::vector<IDescriptor*> descriptors(_DescriptorNbr);

		for (size_t i = 0; i < _DescriptorNbr; ++i)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = _Buffer[i]->CastToVulkan()->GetBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = static_cast<VkDeviceSize>(_BufferSize);

			VkWriteDescriptorSet descriptorWrites{};

			descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites.dstSet = descriptorSets[i];
			descriptorWrites.pNext = nullptr;
			descriptorWrites.dstBinding = 0;
			descriptorWrites.dstArrayElement = 0;
			descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites.descriptorCount = 1;
			descriptorWrites.pBufferInfo = &bufferInfo;
			descriptorWrites.pImageInfo = nullptr;
			descriptorWrites.pTexelBufferView = nullptr;

			vkUpdateDescriptorSets(device.GetLogicalDevice(), 1, &descriptorWrites, 0, nullptr);

			descriptors[i] = new VulkanDescriptor(descriptorSets[i]);
		}

		return descriptors;
	}
}