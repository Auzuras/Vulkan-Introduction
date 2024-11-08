#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandAllocator.h"

namespace Core
{
	const RHI_RESULT VulkanCommandAllocator::CreateCommandAllocator()
	{
		// Gets the queue families
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_PhysicalDevice, m_Surface);

		// Creates the pool infos
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		// Specifies the graphics queue for the command pool
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		// Creates the command pool
		VkResult result = vkCreateCommandPool(m_LogicalDevice, &poolInfo, nullptr, &m_CommandPool);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create command pool, Error Code: %d", result);
		}
	}

	const RHI_RESULT VulkanCommandAllocator::DestroyCommandAllocator()
	{
		return RHI_RESULT();
	}
}