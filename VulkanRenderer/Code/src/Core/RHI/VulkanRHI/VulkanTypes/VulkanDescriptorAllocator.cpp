#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptorAllocator.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"

namespace Core
{

	VulkanDescriptorAllocator::~VulkanDescriptorAllocator()
	{}

	const RHI_RESULT VulkanDescriptorAllocator::CreateDescriptorAllocator(IDevice* _Device, ISwapChain* _Swapchain)
	{
		VulkanDevice device = *_Device->CastToVulkan();
		VulkanSwapChain swapchain = *_Swapchain->CastToVulkan();

		// Describes the pool size
		std::array<VkDescriptorPoolSize, 3> poolSizes{};
		// UBO
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchain.GetSwapchainImagesNbr());
		// UBO
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchain.GetSwapchainImagesNbr());
		// Sampler
		poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[2].descriptorCount = static_cast<uint32_t>(swapchain.GetSwapchainImagesNbr());

		// Pool infos
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		// All Descriptors size
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapchain.GetSwapchainImagesNbr());

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

	std::vector<IDescriptor*> VulkanDescriptorAllocator::CreateDescriptor(IDevice* _Device, int _DescriptorNbr)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		std::vector<VkDescriptorSetLayout> layouts(_DescriptorNbr, m_DescriptorSetLayout);

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
		}

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = yourUniformBuffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(YourUniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = yourImageView;
		imageInfo.sampler = yourSampler;

		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[0];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(device.GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}