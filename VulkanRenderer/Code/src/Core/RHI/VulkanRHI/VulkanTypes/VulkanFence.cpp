#include "RHI/VulkanRHI/VulkanTypes/VulkanFence.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

namespace Core
{
	VulkanFence::~VulkanFence()
	{}

	RHI_RESULT VulkanFence::CreateFenceSync(IDevice * _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Fence create info - for CPU / GPU synchronization
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// Specifies that the fence is created as signaled to avoid an infinite wait at the start of the draw
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkResult result = vkCreateFence(device.GetLogicalDevice(), &fenceInfo, nullptr, &m_Fence);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create fence, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	RHI_RESULT VulkanFence::DestroyFenceSync(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroyFence(device.GetLogicalDevice(), m_Fence, nullptr);

		return RHI_SUCCESS;
	}

	void VulkanFence::WaitFence(IDevice* _Device, unsigned int _CancelDelay)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkWaitForFences(device.GetLogicalDevice(), 1, &m_Fence, VK_TRUE, _CancelDelay);
	}

	void VulkanFence::ResetFence(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkResetFences(device.GetLogicalDevice(), 1, &m_Fence);
	}
}