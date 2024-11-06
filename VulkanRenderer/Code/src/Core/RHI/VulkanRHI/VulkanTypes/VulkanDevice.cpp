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
			if (IsDeviceSuitable(device))
			{
				m_PhysicalDevice = device;
				break;
			}
		}

		if (m_PhysicalDevice == VK_NULL_HANDLE)
		{
			DEBUG_ERROR("Failed to find a suitable GPU");
		}
    }

	const bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice _Device)
	{
		// For the basic properties (Name, Vulkan version supported, Driver version...)
		VkPhysicalDeviceProperties deviceProperties;

		// For optional properties (Compressed textures, 64bits floats, Multi viewport rendering...)
		VkPhysicalDeviceFeatures deviceFeatures;

		// Gets all the infomations
		vkGetPhysicalDeviceProperties(_Device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(_Device, &deviceFeatures);

		// Gets required familie queues
		QueueFamilyIndices indices = FindQueueFamilies(_Device, m_Surface);

		// Checks if all the extensions we need are availables
		bool areExtensionsSupported = CheckDeviceExtensionSupport(_Device);

		bool isSwapChainAdequate = false;

		if (areExtensionsSupported)
		{
			// Checks if the SwapChain is supported by our device
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_Device);
			// Sets the swapchain adequate bool according to the formats and present modes
			isSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
		else
		{
			DEBUG_ERROR("Some extensions required are not supported by the device");
			return false;
		}

		// Return the device if it is a discrete gpu (indeppendent GPU only) and if it supports geometry shader, graphic and presentation queues, extension, swap chains and anisotropy
		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
			&& deviceFeatures.geometryShader
			&& indices.isComplete
			&& areExtensionsSupported
			&& isSwapChainAdequate
			&& deviceFeatures.samplerAnisotropy;
	}

	void VulkanDevice::CreateLogicalDevice()
	{
		// Gets all queue families to reference the number of queues required for our application
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice, m_Surface);
		float queuePriority = 1.0f;

		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		// References our queues in a create info for the device
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// Specifies the features of the device we want to use
		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		// Specifies the information for the logical device (extensions and queues)
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

		// Specifies the validations layers if enabled
		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		// Creates the logical device
		VkResult result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to created logical device, Error Code: %d", result);
			return;
		}

		// References the queues
		vkGetDeviceQueue(m_LogicalDevice, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, indices.presentFamily.value(), 0, &m_PresentQueue);

		return;
	}

    void VulkanDevice::Initialize(VkInstance _VulkanInstance)
    {
		PickPhysicalDevice(_VulkanInstance);
    }

    void VulkanDevice::Terninate()
    {
    }
}