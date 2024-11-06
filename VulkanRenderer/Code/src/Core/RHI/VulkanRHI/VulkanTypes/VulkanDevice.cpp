#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

namespace Core
{
    void VulkanDevice::PickPhysicalDevice(VkInstance _VulkanInstance)
    {
		// Gets the number of GPU
		uint32_t deviceNbr = 0;
		vkEnumeratePhysicalDevices(_VulkanInstance, &deviceNbr, nullptr);

		if (deviceNbr == 0)
		{
			DEBUG_ERROR("Failed to find GPUs with Vulkan support");
		}

		// Gets all the GPU in a list
		std::vector<VkPhysicalDevice> devices(deviceNbr);
		vkEnumeratePhysicalDevices(_VulkanInstance, &deviceNbr, devices.data());

		// Can be change to choose the best GPU according to a score
		for (const VkPhysicalDevice& device : devices)
		{
			//if (IsDeviceSuitable(device))
			//{
			//	m_PhysicalDevice = device;
			//	break;
			//}
		}

		if (m_PhysicalDevice == VK_NULL_HANDLE)
		{
			DEBUG_ERROR("Failed to find a suitable GPU");
		}
    }

	//const bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice _Device)
	//{
	//	// For the basic properties (Name, Vulkan version supported, Driver version...)
	//	VkPhysicalDeviceProperties deviceProperties;

	//	// For optional properties (Compressed textures, 64bits floats, Multi viewport rendering...)
	//	VkPhysicalDeviceFeatures deviceFeatures;

	//	// Gets all the infomations
	//	vkGetPhysicalDeviceProperties(_Device, &deviceProperties);
	//	vkGetPhysicalDeviceFeatures(_Device, &deviceFeatures);

	//	// Gets required familie queues
	//	QueueFamilyIndices indices = FindQueueFamilies(_Device, m_Surface);

	//	// Checks if all the extensions we need are availables
	//	bool areExtensionsSupported = CheckDeviceExtensionSupport(_Device);

	//	bool isSwapChainAdequate = false;

	//	if (areExtensionsSupported)
	//	{
	//		// Checks if the SwapChain is supported by our device
	//		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_Device);
	//		// Sets the swapchain adequate bool according to the formats and present modes
	//		isSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	//	}
	//	else
	//	{
	//		DEBUG_ERROR("Some extensions required are not supported by the device");
	//		return false;
	//	}

	//	// Return the device if it is a discrete gpu (indeppendent GPU only) and if it supports geometry shader, graphic and presentation queues, extension, swap chains and anisotropy
	//	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
	//		&& deviceFeatures.geometryShader
	//		&& indices.isComplete
	//		&& areExtensionsSupported
	//		&& isSwapChainAdequate
	//		&& deviceFeatures.samplerAnisotropy;
	//}

    void VulkanDevice::Initialize(VkInstance _VulkanInstance)
    {
		PickPhysicalDevice(_VulkanInstance);
    }

    void VulkanDevice::Terninate()
    {
    }
}