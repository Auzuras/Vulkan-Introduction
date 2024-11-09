#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandAllocator.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandBuffer.h"

namespace Core
{
	VulkanCommandAllocator::~VulkanCommandAllocator()
	{}

	const RHI_RESULT VulkanCommandAllocator::CreateCommandAllocator(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Gets the queue families
		QueueFamilyIndices queueFamilyIndices;// = FindQueueFamilies(m_PhysicalDevice, m_Surface);

		// Creates the pool infos
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		// Specifies the graphics queue for the command pool
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		// Creates the command pool
		VkResult result = vkCreateCommandPool(device.GetLogicalDevice(), &poolInfo, nullptr, &m_CommandPool);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create command pool, Error Code: %d", result);
		}
	}

	const RHI_RESULT VulkanCommandAllocator::DestroyCommandAllocator(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroyCommandPool(device.GetLogicalDevice(), m_CommandPool, nullptr);
		return RHI_RESULT();
	}

	ICommandBuffer* VulkanCommandAllocator::CreateCommandBuffer(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		// Specifies the command pool
		allocInfo.commandPool = m_CommandPool;
		// Specifies if the command buffer is Primary or Secondary
		// Primary : Can be sent into a queue but can't be called by other command buffers
		// Secondary : Can be called from primary command buffers but can't be directly sent to a queue
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		// Specifies the number of command buffer to create
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;

		// Creates the command buffer
		VkResult result = vkAllocateCommandBuffers(device.GetLogicalDevice(), &allocInfo, &commandBuffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate command buffers, Error Code: %d", result);
			return nullptr;
		}

		VulkanCommandBuffer* vkCommandBuffer = new VulkanCommandBuffer(commandBuffer);

		return vkCommandBuffer;
	}

	std::vector<ICommandBuffer*> VulkanCommandAllocator::CreateCommandBuffers(IDevice* _Device, uint32_t _CommandBuffersNbr)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		// Specifies the command pool
		allocInfo.commandPool = m_CommandPool;
		// Specifies if the command buffer is Primary or Secondary
		// Primary : Can be sent into a queue but can't be called by other command buffers
		// Secondary : Can be called from primary command buffers but can't be directly sent to a queue
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		// Specifies the number of command buffer to create
		allocInfo.commandBufferCount = _CommandBuffersNbr;

		std::vector<VkCommandBuffer> cmdBuffers(_CommandBuffersNbr);

		// Creates the command buffer
		VkResult result = vkAllocateCommandBuffers(device.GetLogicalDevice(), &allocInfo, cmdBuffers.data());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate command buffers, Error Code: %d", result);
		}

		std::vector<ICommandBuffer*> finalCmdBuffers;

		for (uint32_t i = 0; i < _CommandBuffersNbr; ++i)
		{
			finalCmdBuffers.push_back(new VulkanCommandBuffer(cmdBuffers[i]));
		}

		return finalCmdBuffers;
	}
}