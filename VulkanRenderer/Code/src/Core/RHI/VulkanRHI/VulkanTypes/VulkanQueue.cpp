#include "RHI/VulkanRHI/VulkanTypes/VulkanQueue.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

namespace Core
{
	RHI_RESULT VulkanQueue::CreateQueue()
	{
		return RHI_RESULT();
	}

	RHI_RESULT VulkanQueue::DestroyQueue()
	{
		return RHI_RESULT();
	}

	QueueFamilyIndices VulkanQueue::FindQueueFamilies(VulkanDevice _Device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyNbr = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_Device.GetPhysicalDevice(), &queueFamilyNbr, nullptr);

		// Gets all queue families of the gpu
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyNbr);
		vkGetPhysicalDeviceQueueFamilyProperties(_Device.GetPhysicalDevice(), &queueFamilyNbr, queueFamilies.data());

		int index = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies)
		{
			// Checks for a queue supporting transfer queues but that is not a graphics queue
			if (!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT))
			{
				indices.transfertFamily = index;
			}

			// Checks for a queue supporting graphic queues
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = index;
			}

			// Checks if the current queue can support presentation queues 
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_Device.GetPhysicalDevice(), index, _Device.GetSurface(), &presentSupport);

			// Reference the queue if it can support presentation
			if (presentSupport)
			{
				indices.presentFamily = index;
			}

			// Return the queues if graphics and presentation queues have been found
			if (indices.graphicsFamily.has_value() && indices.presentFamily.has_value())
			{
				break;
			}

			++index;
		}

		// bool describing if the two types of families are supported on our device
		indices.isComplete = indices.graphicsFamily.has_value() && indices.presentFamily.has_value() && indices.transfertFamily.has_value();

		return indices;
	}
}
