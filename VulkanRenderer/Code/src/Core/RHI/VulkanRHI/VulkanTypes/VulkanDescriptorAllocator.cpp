#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptorAllocator.h"

namespace Core
{

	VulkanDescriptorAllocator::~VulkanDescriptorAllocator()
	{}

	const RHI_RESULT VulkanDescriptorAllocator::CreateDescriptorAllocator(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Describes the pool size
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		// UBO
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		// Sampler
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		// Pool infos
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		// All Descriptors size
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

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
}