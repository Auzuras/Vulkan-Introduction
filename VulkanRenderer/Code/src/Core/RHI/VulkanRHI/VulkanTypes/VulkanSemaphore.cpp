#include "RHI/VulkanRHI/VulkanTypes/VulkanSemaphore.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

namespace Core
{
	VulkanSemaphore::~VulkanSemaphore()
	{}

	RHI_RESULT VulkanSemaphore::CreateSemaphoreSync(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Semaphore create info - for GPU/ GPU synchronization
		VkSemaphoreCreateInfo smaphoreInfo{};
		smaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		// Creates Image available semaphores
		VkResult result = vkCreateSemaphore(device.GetLogicalDevice(), &smaphoreInfo, nullptr, &m_Semaphore);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create semaphore, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	RHI_RESULT VulkanSemaphore::DestroySemaphoreSync(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroySemaphore(device.GetLogicalDevice(), m_Semaphore, nullptr);
		return RHI_SUCCESS;
	}
}