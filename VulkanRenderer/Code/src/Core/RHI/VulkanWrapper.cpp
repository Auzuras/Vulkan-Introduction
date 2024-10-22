#include "RHI/VulkanWrapper.h"

#include "Debug/Log.h"

#include <cstring>

namespace Core
{
	const bool VulkanWrapper::Initialize()
	{
		if (!CreateVulkanInstance())
			return false;

		SetupDebugMessenger();
		PickPhysicalDevice();


		return true;
	}

	const bool VulkanWrapper::CreateVulkanInstance()
	{
		if (m_EnableValidationLayers && !CheckValidationLayerSupport())
		{
			DEBUG_WARN("Validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Gets all extensions
		// TODO: Move this outside of the vulkan initialization \/

		std::vector<const char*> requiredExtensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();

		//// /\
		
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		}
		else 
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create Vulkan instance, Error Code: %d", result);
			return false;
		}

		return true;
	}

	const bool VulkanWrapper::SetupDebugMessenger()
	{
		if (!m_EnableValidationLayers)
			return false;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		VkResult result = CreateDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr, &m_DebugMessenger);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to set up debug messenger, Error Code: %d", result);
			return false;
		}

		return true;
	}

	const bool VulkanWrapper::Terminate()
	{
		vkDestroyDevice(m_LogicalDevice, nullptr);

		if (m_EnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_VulkanInstance, m_DebugMessenger, nullptr);
		}

		vkDestroyInstance(m_VulkanInstance, nullptr);
		return true;
	}

	const bool VulkanWrapper::CheckValidationLayerSupport()
	{
		uint32_t layerNbr;
		vkEnumerateInstanceLayerProperties(&layerNbr, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerNbr);
		vkEnumerateInstanceLayerProperties(&layerNbr, availableLayers.data());

		for (const char* layerName : m_ValidationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}

		}

		return true;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanWrapper::DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT _MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT _MessageType, const VkDebugUtilsMessengerCallbackDataEXT* _CallbackData, void* _UserData)
	{
		switch (_MessageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: default:
			DEBUG_LOG("VK Validation layer: %s", _CallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			DEBUG_WARN("VK Validation layer: %s", _CallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			DEBUG_ERROR("VK Validation layer: %s", _CallbackData->pMessage);
			break;
		}

		DEBUG_LOG("Validation layer: %s", _CallbackData->pMessage);

		return VK_FALSE;
	}

	VkResult VulkanWrapper::CreateDebugUtilsMessengerEXT(VkInstance _Instance, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo, const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_Instance, "vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr)
		{
			return func(_Instance, _CreateInfo, _Allocator, _DebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanWrapper::DestroyDebugUtilsMessengerEXT(VkInstance _Instance, VkDebugUtilsMessengerEXT _DebugMessenger, const VkAllocationCallbacks* _Allocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_Instance, "vkDestroyDebugUtilsMessengerEXT");

		if (func != nullptr)
		{
			func(_Instance, _DebugMessenger, _Allocator);
		}
		else
		{
			DEBUG_ERROR("Failed to destroy debug messenger");
		}
	}

	void VulkanWrapper::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _CreateInfo)
	{
		_CreateInfo = {};
		_CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		_CreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		_CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		_CreateInfo.pfnUserCallback = DebugCallBack;
	}

	std::vector<const char*> VulkanWrapper::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (m_EnableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void VulkanWrapper::PickPhysicalDevice()
	{
		uint32_t deviceNbr = 0;
		vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceNbr, nullptr);

		if (deviceNbr == 0)
		{
			DEBUG_ERROR("Failed to find GPUs with Vulkan support");
		}

		vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceNbr, nullptr);

		std::vector<VkPhysicalDevice> devices(deviceNbr);
		vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceNbr, devices.data());

		// Can be change to choose a device according to a score
		for (VkPhysicalDevice device : devices)
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

	const bool VulkanWrapper::IsDeviceSuitable(VkPhysicalDevice _Device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(_Device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(_Device, &deviceFeatures);

		QueueFamilyIndices indices = FindQueueFamilies(_Device);

		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader && indices.isComplete;
	}

	QueueFamilyIndices VulkanWrapper::FindQueueFamilies(VkPhysicalDevice _Device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyNbr = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &queueFamilyNbr, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyNbr);
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &queueFamilyNbr, queueFamilies.data());

		int index = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = index;
			}

			if (indices.graphicsFamily.has_value())
			{
				break;
			}

			++index;
		}

		indices.isComplete = indices.graphicsFamily.has_value();

		return indices;
	}

	void VulkanWrapper::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
		float queuePriority = 1.0f;

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;

		createInfo.pEnabledFeatures = &deviceFeatures;

		VkResult result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to created logical device, Error Code: %d", result);
			return;
		}

		vkGetDeviceQueue(m_LogicalDevice, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);

		return;
	}
}