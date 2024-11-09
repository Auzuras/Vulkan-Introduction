#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanQueue.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"

#include <set>

namespace Core
{
	const RHI_RESULT VulkanDevice::CreateVulkanInstance()
	{
		// Checks if validation layers are available if requested
		if (m_EnableValidationLayers && !CheckValidationLayerSupport())
		{
			DEBUG_WARN("Validation layers requested, but not available!");
			return RHI_FAILED_UNKNOWN;
		}

		// Create the app
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan Renderer";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Gets all extensions and specifies it
		std::vector<const char*> requiredExtensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();

		// Specifies or not the validation layers used in the application
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

			// Allows to debug the creation and destruction of the instance
			// Because the debug messenger will be created after the instance and destroyed before the instance
			// By referencing a create info for the debugger debug the instance
			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		// Creates the vulkan instance
		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create Vulkan instance, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	const RHI_RESULT VulkanDevice::SetupDebugMessenger()
	{
		if (!m_EnableValidationLayers)
		{
			DEBUG_WARN("Debug messenger can't be created because validation layers are desactivated");
			return RHI_FAILED_UNKNOWN;
		}

		// Setups the DebugMessenger and the callback function
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		// Creates the debug messenger
		VkResult result = CreateDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr, &m_DebugMessenger);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to set up debug messenger, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	const bool VulkanDevice::CheckValidationLayerSupport()
	{
		// Gets the list of all layer supported
		uint32_t layerNbr;
		vkEnumerateInstanceLayerProperties(&layerNbr, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerNbr);
		vkEnumerateInstanceLayerProperties(&layerNbr, availableLayers.data());

		// Runs the loop for each layers we want to activate
		for (const char* layerName : m_ValidationLayers)
		{
			bool layerFound = false;

			// Checks if validation layers are present in all the layer supported
			for (const auto& layerProperties : availableLayers)
			{
				// Checks the equality of to strings, here we stop the loop if validation layers are supported
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

	void VulkanDevice::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _CreateInfo)
	{
		// Specifies all the information for the debug messenger
		_CreateInfo = {};
		_CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		// Specifies what type/severity of messages we want the callback function to be called for
		_CreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		_CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		// Links the callback function to the debug messenger
		_CreateInfo.pfnUserCallback = DebugCallBack;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDevice::DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT _MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT _MessageType, const VkDebugUtilsMessengerCallbackDataEXT* _CallbackData, void* _UserData)
	{
		// Different message type according to its severity
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

		return VK_FALSE;
	}

	VkResult VulkanDevice::CreateDebugUtilsMessengerEXT(VkInstance _Instance, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo, const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger)
	{
		// Loads the Debug Messenger creation method - Because validation layers are an extension we need to load manually the method
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_Instance, "vkCreateDebugUtilsMessengerEXT");

		// Use the method if not null
		if (func != nullptr)
		{
			return func(_Instance, _CreateInfo, _Allocator, _DebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanDevice::DestroyDebugUtilsMessengerEXT(VkInstance _Instance, VkDebugUtilsMessengerEXT _DebugMessenger, const VkAllocationCallbacks* _Allocator)
	{
		// Loads the Debug Messenger deletion method - Because validation layers are an extension we need to load manually the method
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_Instance, "vkDestroyDebugUtilsMessengerEXT");

		// Use the method if not null
		if (func != nullptr)
		{
			func(_Instance, _DebugMessenger, _Allocator);
		}
		else
		{
			DEBUG_ERROR("Failed to destroy debug messenger");
		}
	}

	const RHI_RESULT VulkanDevice::PickPhysicalDevice()
    {
		// Gets the number of GPU
		uint32_t deviceNbr = 0;
		vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceNbr, nullptr);

		if (deviceNbr == 0)
		{
			DEBUG_ERROR("Failed to find GPUs with Vulkan support");
			return RHI_FAILED_UNKNOWN;
		}

		// Gets all the GPU in a list
		std::vector<VkPhysicalDevice> devices(deviceNbr);
		vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceNbr, devices.data());

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
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
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
		QueueFamilyIndices indices = VulkanQueue::FindQueueFamilies(*this);

		// Checks if all the extensions we need are availables
		bool areExtensionsSupported = CheckDeviceExtensionSupport(_Device);

		bool isSwapChainAdequate = false;

		if (areExtensionsSupported)
		{
			// Checks if the SwapChain is supported by our device
			SwapChainSupportDetails swapChainSupport = VulkanSwapChain::QuerySwapChainSupport(*this);
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

	const RHI_RESULT VulkanDevice::CreateLogicalDevice()
	{
		// Gets all queue families to reference the number of queues required for our application
		QueueFamilyIndices indices = VulkanQueue::FindQueueFamilies(*this);
		float queuePriority = 1.0f;

		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value(), indices.transfertFamily.value() };

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
			return RHI_FAILED_UNKNOWN;
		}

		// References the queues
		vkGetDeviceQueue(m_LogicalDevice, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, indices.presentFamily.value(), 0, &m_PresentationQueue);
		vkGetDeviceQueue(m_LogicalDevice, indices.transfertFamily.value(), 0, &m_TransferQueue);

		return RHI_SUCCESS;
	}

	void VulkanDevice::CreateSurface(Window* _Window)
	{
		// Gets the window surface for vulkan
		VkResult result = glfwCreateWindowSurface(m_VulkanInstance, _Window->GetWindowPointer(), nullptr, &m_Surface);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create window surface, Error Code: %d", result);
		}
	}

	std::vector<const char*> VulkanDevice::GetRequiredExtensions()
	{
		// Gets GLFW extensions in order to link our window with vulkan
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		// Suplementary extension needed for debugging and validation layers
		if (m_EnableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	const bool VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice _Device)
	{
		// Gets the number of extensions supported by our device
		uint32_t extensionsNbr;
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &extensionsNbr, nullptr);

		// Gets all the extensions supported by our device
		std::vector<VkExtensionProperties> availableExtensions(extensionsNbr);
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &extensionsNbr, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

		// Erase the extensions required by our application if the extension is available in our device
		// If the requiredExtensions list is empty it means that all the required extensions are available
		for (const VkExtensionProperties& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);

			if (requiredExtensions.empty())
				return requiredExtensions.empty();
		}

		// Print all extensions not supported
		for (const std::string& extension : requiredExtensions)
		{
			DEBUG_WARN("Extension not supported: %s", extension);
		}

		return requiredExtensions.empty();
	}

	VulkanDevice::~VulkanDevice()
	{}

    const RHI_RESULT VulkanDevice::Initialize(Window* _Window)
    {
		CreateVulkanInstance();
		CreateSurface(_Window);
		PickPhysicalDevice();
		CreateLogicalDevice();

		return RHI_SUCCESS;
    }

	const RHI_RESULT VulkanDevice::Terminate()
    {
		vkDestroyDevice(m_LogicalDevice, nullptr);

		if (m_EnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_VulkanInstance, m_DebugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);
		vkDestroyInstance(m_VulkanInstance, nullptr);

		return RHI_SUCCESS;
    }
}