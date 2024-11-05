#include "RHI/VulkanWrapper.h"

#include <cstring>
#include <set>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Utils/Utils.h"

namespace Core
{
	VulkanWrapper::~VulkanWrapper()
	{}

	const bool VulkanWrapper::Initialize(GLFWwindow* _Window)
	{
		if (!CreateVulkanInstance())
			return false;

		// Defines the window resize callback with swapchain recreation
		// Adds a custom pointer to our class to access it in any call back functions
		glfwSetWindowUserPointer(_Window, this);
		glfwSetFramebufferSizeCallback(_Window, FrameBufferResizeCallback);

		SetupDebugMessenger();
		CreateSurface(_Window);
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapChain(_Window);
		CreateSwapChainImageViews();
		CreateRenderPass();
		CreateDescriptorSetLayout();
		CreateGraphicsPipeline();
		CreateCommandPool();
		CreateDepthRessources();
		CreateSwapChainFramebuffers();
		CreateTextureImage();
		CreateTextureImageView();
		CreateTextureSampler();
		LoadModel();
		CreateVertexBuffer();
		CreateIndexBuffer();
		CreateUniformBuffers();
		CreateDescriptorPool();
		CreateDescriptorSets();
		CreateCommandBuffers();
		CreateSyncObjects();

		return true;
	}

	const bool VulkanWrapper::CreateVulkanInstance()
	{
		// Checks if validation layers are available if requested
		if (m_EnableValidationLayers && !CheckValidationLayerSupport())
		{
			DEBUG_WARN("Validation layers requested, but not available!");
		}

		// Create the app
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
			return false;
		}

		return true;
	}

	const bool VulkanWrapper::SetupDebugMessenger()
	{
		if (!m_EnableValidationLayers)
		{
			DEBUG_WARN("Debug messenger can't be created because validation layers are desactivated");
			return false;
		}

		// Setups the DebugMessenger and the callback function
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		// Creates the debug messenger
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
		// Destroys every objects created

		vkDeviceWaitIdle(m_LogicalDevice);

		CleanSwapChain();

		vkDestroySampler(m_LogicalDevice, m_TextureSampler, nullptr);
		vkDestroyImageView(m_LogicalDevice, m_TextureImageView, nullptr);

		vkDestroyImage(m_LogicalDevice, m_TextureImage, nullptr);
		vkFreeMemory(m_LogicalDevice, m_TextureImageMemory, nullptr);

		vkDestroyPipeline(m_LogicalDevice, m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_LogicalDevice, m_PipelineLayout, nullptr);
		vkDestroyRenderPass(m_LogicalDevice, m_RenderPass, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			vkDestroyBuffer(m_LogicalDevice, m_UniformBuffers[i], nullptr);
			vkFreeMemory(m_LogicalDevice, m_UniformBufferMemory[i], nullptr);
		}

		vkDestroyDescriptorPool(m_LogicalDevice, m_DescriptorPool, nullptr);

		vkDestroyDescriptorSetLayout(m_LogicalDevice, m_DescriptorSetLayout, nullptr);

		vkDestroyBuffer(m_LogicalDevice, m_IndexBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, m_IndexBufferMemory, nullptr);

		vkDestroyBuffer(m_LogicalDevice, m_VertexBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, m_VertexBufferMemory, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			vkDestroySemaphore(m_LogicalDevice, m_ImageAvailableSemaphores[i], nullptr);

			vkDestroySemaphore(m_LogicalDevice, m_RenderFinishedSemaphores[i], nullptr);

			vkDestroyFence(m_LogicalDevice, m_InFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);

		vkDestroyDevice(m_LogicalDevice, nullptr);

		if (m_EnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_VulkanInstance, m_DebugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);
		vkDestroyInstance(m_VulkanInstance, nullptr);

		return true;
	}

	const bool VulkanWrapper::CheckValidationLayerSupport()
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

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanWrapper::DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT _MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT _MessageType, const VkDebugUtilsMessengerCallbackDataEXT* _CallbackData, void* _UserData)
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

	VkResult VulkanWrapper::CreateDebugUtilsMessengerEXT(VkInstance _Instance, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo, const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger)
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

	void VulkanWrapper::DestroyDebugUtilsMessengerEXT(VkInstance _Instance, VkDebugUtilsMessengerEXT _DebugMessenger, const VkAllocationCallbacks* _Allocator)
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

	void VulkanWrapper::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _CreateInfo)
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

	std::vector<const char*> VulkanWrapper::GetRequiredExtensions()
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

	void VulkanWrapper::PickPhysicalDevice()
	{
		// Gets the number of GPU
		uint32_t deviceNbr = 0;
		vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceNbr, nullptr);

		if (deviceNbr == 0)
		{
			DEBUG_ERROR("Failed to find GPUs with Vulkan support");
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
		}

	}

	const bool VulkanWrapper::IsDeviceSuitable(VkPhysicalDevice _Device)
	{
		// For the basic properties (Name, Vulkan version supported, Driver version...)
		VkPhysicalDeviceProperties deviceProperties;

		// For optional properties (Compressed textures, 64bits floats, Multi viewport rendering...)
		VkPhysicalDeviceFeatures deviceFeatures;

		// Gets all the infomations
		vkGetPhysicalDeviceProperties(_Device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(_Device, &deviceFeatures);

		// Gets required familie queues
		QueueFamilyIndices indices = FindQueueFamilies(_Device);

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

	QueueFamilyIndices VulkanWrapper::FindQueueFamilies(VkPhysicalDevice _Device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyNbr = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &queueFamilyNbr, nullptr);

		// Gets all queue families of the gpu
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyNbr);
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &queueFamilyNbr, queueFamilies.data());

		int index = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies)
		{
			// Checks for a queue supporting graphic queues
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = index;
			}

			// Checks if the current queue can support presentation queues 
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_Device, index, m_Surface, &presentSupport);

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
		indices.isComplete = indices.graphicsFamily.has_value() && indices.presentFamily.has_value();

		return indices;
	}

	void VulkanWrapper::CreateLogicalDevice()
	{
		// Gets all queue families to reference the number of queues required for our application
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
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

	void VulkanWrapper::CreateSurface(GLFWwindow* _Window)
	{
		// Gets the window surface for vulkan
		VkResult result = glfwCreateWindowSurface(m_VulkanInstance, _Window, nullptr, &m_Surface);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create window surface, Error Code: %d", result);
		}
	}

	const bool VulkanWrapper::CheckDeviceExtensionSupport(VkPhysicalDevice _Device)
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

	SwapChainSupportDetails VulkanWrapper::QuerySwapChainSupport(VkPhysicalDevice _Device)
	{
		SwapChainSupportDetails details;

		// Checks the surfaces capabilities (min / max frames in the swap chain, min / max - height / width of the images)
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_Device, m_Surface, &details.capabilities);

		// Checks the number of format available (pixels format / colors)
		uint32_t formatNbr;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, m_Surface, &formatNbr, nullptr);

		// References the formats if availables
		if (formatNbr != 0)
		{
			details.formats.resize(formatNbr);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, m_Surface, &formatNbr, details.formats.data());
		}

		// Checks the number of presentations modes (FIFO / Mailbox...)
		uint32_t presentModeNbr;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, m_Surface, &presentModeNbr, nullptr);

		// References the presentations modes if availables
		if (presentModeNbr != 0)
		{
			details.presentModes.resize(presentModeNbr);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, m_Surface, &presentModeNbr, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VulkanWrapper::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _AvailableFormats)
	{
		for (const VkSurfaceFormatKHR& format : _AvailableFormats)
		{
			// Checks for BGR 8 bits format and standard RGB colorSpace in all of our formats
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return format;
			}
		}

		// If not found return the first format available
		return _AvailableFormats[0];
	}

	VkPresentModeKHR VulkanWrapper::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _AvailablePresentModes)
	{
		for (const VkPresentModeKHR& presentMode : _AvailablePresentModes)
		{
			// Checks for Mailbox presentation mode in all the available presentation modes
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return presentMode;
			}
		}

		// If not found return FIFO presentation mode
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanWrapper::ChooseSwapExtent(GLFWwindow* _Window, const VkSurfaceCapabilitiesKHR& _Capabilities)
	{
		// This condition checks if the currentExtent is fixed by the system or the user
		if (_Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			// The first case here is if the user defined a size
			return _Capabilities.currentExtent;
		}
		else
		{
			// The second case here is if the user doesn't define a size so the system does
			// In this case the currentExent will be equal to max of uint32_t
			int width, height;
			glfwGetFramebufferSize(_Window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			// Create an extent of the size of the GLFW frame buffer and clamp it if it below or above the capabilities of the swap chain
			actualExtent.width = std::clamp(actualExtent.width, _Capabilities.minImageExtent.width, _Capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, _Capabilities.minImageExtent.height, _Capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void VulkanWrapper::CreateSwapChain(GLFWwindow* _Window)
	{
		// Gets all the capabilities and information the swapchain can support
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_PhysicalDevice);

		// Selects the best surface format
		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);

		// Selects the best presentation mode
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);

		// Selects the extent
		VkExtent2D extent = ChooseSwapExtent(_Window, swapChainSupport.capabilities);

		// Selects how much images we want in the swap chain - Here the minimum + 1 to avoid any "pause" in the program
		uint32_t imageNbr = swapChainSupport.capabilities.minImageCount + 1;

		// If maxImageCount is equal to 0 it means that the only limit is the memory
		// Here if the number of images is above the max capabilities it is just clamped to the max
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageNbr > swapChainSupport.capabilities.maxImageCount)
		{
			imageNbr = swapChainSupport.capabilities.maxImageCount;
		}

		// Create info of the swapchain
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = imageNbr;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		// imageArrayLayers is the number of layers an image possess - only usefull for 3D stereoscopic applications
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		
		// Gets all queue families to setup up sharing mode of the images in the swap chain
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		// If there is two different queues with one for graphic and the other for presenting we need to share the images
		if (indices.graphicsFamily != indices.presentFamily)
		{
			// Shares the images between two different queues
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			// Do not share the images because there is only one queue doing graphic and presentation
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		// Specifies a modification on the image if needed (Rotation / Flip)
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		// Specifies if the alpha component of the window should blend with the other windows
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		// Speicifies if the pixels hidden by other windows are discarded
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		m_SwapChainImageFormat = surfaceFormat.format;
		m_SwapChainExtent = extent;

		// Creates the swap chain
		VkResult result = vkCreateSwapchainKHR(m_LogicalDevice, &createInfo, nullptr, &m_SwapChain);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create swap chain, Error Code: %d", result);
			return;
		}

		// Gets all the images of the swap chain
		vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageNbr, nullptr);
		m_SwapChainImages.resize(imageNbr);
		vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageNbr, m_SwapChainImages.data());

		return;
	}

	void VulkanWrapper::RecreateSwapChain(GLFWwindow* _Window)
	{
		// Gets the current size of the window
		int width = 0, height = 0;
		glfwGetFramebufferSize(_Window, &width, &height);

		// Loop executed when the window is minimized (when width and height = 0)
		while (width == 0 || height == 0)
		{
			// Updates the size to get out when not minimized
			glfwGetFramebufferSize(_Window, &width, &height);

			// Pause all events
			glfwWaitEvents();
		}

		// Waits the end of the utilisation of all resources to not access them when used
		vkDeviceWaitIdle(m_LogicalDevice);

		// Destroys the swap chain
		CleanSwapChain();

		// Recreates the swap chain
		CreateSwapChain(_Window);
		CreateSwapChainImageViews();
		CreateDepthRessources();
		CreateSwapChainFramebuffers();
	}

	void VulkanWrapper::CleanSwapChain()
	{
		// Destroys data linked to the swap chain
		vkDestroyImageView(m_LogicalDevice, m_DepthImageView, nullptr);
		vkDestroyImage(m_LogicalDevice, m_DepthImage, nullptr);
		vkFreeMemory(m_LogicalDevice, m_DepthImageMemory, nullptr);

		for (auto framebuffer : m_SwapChainFramebuffers)
		{
			vkDestroyFramebuffer(m_LogicalDevice, framebuffer, nullptr);
		}

		for (VkImageView imageView : m_SwapChainImageViews)
		{
			vkDestroyImageView(m_LogicalDevice, imageView, nullptr);
		}

		// Destroys the swap chain
		vkDestroySwapchainKHR(m_LogicalDevice, m_SwapChain, nullptr);
	}

	void VulkanWrapper::CreateSwapChainImageViews()
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); ++i)
		{
			// Creates an image view for each image of the swap chain
			m_SwapChainImageViews[i] = CreateImageView(m_SwapChainImages[i], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		return;
	}

	void VulkanWrapper::CreateGraphicsPipeline()
	{
		// Compiles and create the two shader modules corresponding to the vertex and fragment shader
		VkShaderModule vertShaderModule = CreateShaderModule(CompileShader("Assets/Shaders/HelloTriangle.vert", ShaderType::VERTEX));
		VkShaderModule fragShaderModule = CreateShaderModule(CompileShader("Assets/Shaders/HelloTriangle.frag", ShaderType::FRAGMENT));

		// Creates vertex shader infos
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		// Stage of the pipeline (Which momement the shader will be called)
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		// Shader module
		vertShaderStageInfo.module = vertShaderModule;
		// Start function of the shader
		vertShaderStageInfo.pName = "main";

		// Creates fragment shader infos
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		// Stage of the pipeline (Which momement the shader will be called)
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		// Shader module
		fragShaderStageInfo.module = fragShaderModule;
		// Start function of the shader
		fragShaderStageInfo.pName = "main";

		// List of all the shaders of the pipeline
		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		// Gets the vertex binding and attribute descriptions
		VkVertexInputBindingDescription bindingDescription = Core::Vertex::GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = Core::Vertex::GetAttributeDescriptions();

		// Describes how the vertex will be inputted in the first shader 
		// (Correspond to the layout(binding=0) position etc in the shader)
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		// How much vertices we are describing
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		// The number of attribute per vertex (Position, Color, Normal ...)
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		// The binding description ()
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		// The attributes description (Binding, size, offset, type)
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		// Describes the geometry
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		// Describes how the geometry should be drawn
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		// Can cancel some links between vertices
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Reference all dynamics states
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		// Dynamic states infos
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		// Rasterizer informations
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		// Fargments above a under the near and far plane are clamped
		rasterizer.depthClampEnable = VK_FALSE;
		// Desactivates rasterizer and steps after
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		// Defines how the polygons are drawn (Fill, poins, lines)
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.f;
		// Defines culling
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		// Modifies the depth acording to a fragment (usefull for shadow mapping)
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.f;
		rasterizer.depthBiasClamp = 0.f;
		rasterizer.depthBiasSlopeFactor = 0.f;

		// Multisampling infos
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		// Activates or not the multisampling
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		// Activate depth test
		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		// Comparison test to eleminate or keep frag
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;

		// Color blending infos (The color given by the fragshader is blend with the other color at the same fragment)
		// This structure describes how it should be blend
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// Specifies how the color blend with many frame buffers
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.f;
		colorBlending.blendConstants[1] = 0.f;
		colorBlending.blendConstants[2] = 0.f;
		colorBlending.blendConstants[3] = 0.f;

		// Pipeline layout infos
		// Describes all uniform buffers present in our pipeline
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		// References the Descriptors set layout (UBO) or global variables
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		// Creates the pipeline layout
		VkResult result = vkCreatePipelineLayout(m_LogicalDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create pipeline layout, Error Code: %d", result);
		}

		// References all the information created
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		// Describes the number of stages (shaders in our pipeline)
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		// Specifies all the information previously created
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		// Describes the layout of the pipeline
		pipelineInfo.layout = m_PipelineLayout;
		// Describes the render pass
		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;
		// Reference a new pipeline to create from this one (not necessary here)
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		// Creates the pipeline
		result = vkCreateGraphicsPipelines(m_LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create graphics pipeline, Error Code: %d", result);
		}

		// Destroys the shader module because they are loaded on the GPU
		vkDestroyShaderModule(m_LogicalDevice, fragShaderModule, nullptr);
		vkDestroyShaderModule(m_LogicalDevice, vertShaderModule, nullptr);
	}

	std::vector<char> VulkanWrapper::ReadSpirVShader(const std::filesystem::path& _FilePath)
	{
		// Cancels the reading if the file is not a SpirV file 
		if (_FilePath.extension() != ".spv")
		{
			DEBUG_ERROR("Wrong shader file extension");
			std::vector<char> err(0);
			return err;
		}

		std::ifstream file(_FilePath, std::ios::ate | std::ios::binary);

		// Cancels the reading if the file failed to open
		if (!file.is_open())
		{
			DEBUG_ERROR("Failed to open shader file");
			std::vector<char> err(0);
			return err;
		}

		// Reads the file
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		// Closes the file
		file.close();

		// Returns the buffer filled by the file
		return buffer;
	}

	VkShaderModule VulkanWrapper::CreateShaderModule(const std::vector<uint32_t>& _ShaderSourceCode)
	{
		// Create info of the shader
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		// Specifies the size and the code of the source code
		createInfo.codeSize = _ShaderSourceCode.size() * sizeof(uint32_t);
		createInfo.pCode = _ShaderSourceCode.data();

		// Creates the shader module
		VkShaderModule shaderModule;
		VkResult result = vkCreateShaderModule(m_LogicalDevice, &createInfo, nullptr, &shaderModule);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create shader module, Error Code: %d", result);
		}

		return shaderModule;
	}

	std::vector<uint32_t> VulkanWrapper::CompileShader(std::filesystem::path _ShaderPath, ShaderType _ShaderType)
	{
		// Causes memory leaks, thx
		shaderc::Compiler compiler;

		// TODO: Move shader reading in the resource loader
		// Reads the shader
		std::ifstream shaderFile(_ShaderPath);
		std::string line = "";
		std::string shaderCode = "";

		if (!shaderFile.is_open())
		{
			DEBUG_ERROR("Failed to open shader");
		}

		while (std::getline(shaderFile, line))
			shaderCode += line + "\n";

		shaderFile.close();

		// Fill a struct of informations about the shader
		CompilationInfos infos{};
		infos.fileName = _ShaderPath.filename().string().c_str();

		switch (_ShaderType)
		{
		case VERTEX: default:
			infos.shaderKind = shaderc_vertex_shader;
			break;
		case FRAGMENT:
			infos.shaderKind = shaderc_fragment_shader;
			break;
		}

		//infos.options.SetOptimizationLevel(shaderc_optimization_level_performance);
		//infos.options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		infos.sourceCode = &shaderCode;

		// First step - Preprocessing GLSL
		shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(*infos.sourceCode, infos.shaderKind, infos.fileName, infos.options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to preprocessed shader, %s", result.GetErrorMessage())
		}

		// Copy the precompiled code in a buffer
		const char* src = result.cbegin();
		size_t newSize = result.cend() - src;
		infos.sourceCode->resize(newSize);
		memcpy(infos.sourceCode->data(), src, newSize);

		// Second step - SPIR-V Assembly compilation
		shaderc::AssemblyCompilationResult sResult = compiler.CompileGlslToSpvAssembly(*infos.sourceCode, infos.shaderKind, infos.fileName, infos.options);

		if (sResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to compiled in assembly shader, %s", sResult.GetErrorMessage())
		}

		// Copy the SPRIV Assembly code in a buffer
		src = sResult.cbegin();
		newSize = sResult.cend() - src;
		infos.sourceCode->resize(newSize);
		memcpy(infos.sourceCode->data(), src, newSize);

		// Third step - SPIR-V Binary compilation
		shaderc::SpvCompilationResult tResult = compiler.AssembleToSpv(infos.sourceCode->data(), infos.sourceCode->size(), infos.options);

		if (tResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to compiled in binary shader, %s", tResult.GetErrorMessage())
		}

		// Copy the SPIRV Binary code in the output buffer
		const uint32_t* srcBinary = tResult.cbegin();
		size_t wordCount = tResult.cend() - srcBinary;
		std::vector<uint32_t> output(wordCount);
		memcpy(output.data(), srcBinary, wordCount * sizeof(uint32_t));

		return output;
	}

	void VulkanWrapper::CreateRenderPass()
	{
		// Describes the color buffer attachment
		VkAttachmentDescription colorAttachment{};
		// Same format as the swap chain
		colorAttachment.format = m_SwapChainImageFormat;
		// Samples (usefull for multisampling)
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		// LoadOp and StoreOp describes how we interact with the data of the buffer before loading and after rendering
		// VK_ATTACHMENT_LOAD_OP_CLEAR will clear all the information with a constant
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		// The rendered frame will be stored
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		// For the stencil buffer we juste dont care about what will be load and unload
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		// 
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Describe the color attachment
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Describes the depth buffer attachment - Check above for more infos (Similar to color attachment)
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = FindDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Describe the depth attachment
		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Creates the subpass with color and depth attachment
		VkSubpassDescription subpass{};
		// Defines the subpass as a graphic one (can be a compute one)
		// Binds the depth attachment to the subpass
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		// Stocks the attachments in a list
		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		// Create info of the render pass
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		// Nbr of attachments
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		// Data of attachments
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		// Creates the render pass
		VkResult result = vkCreateRenderPass(m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create render pass, Error Code: %d", result);
		}
	}

	void VulkanWrapper::CreateSwapChainFramebuffers()
	{
		m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

		// Loop going through every image view of the swap chain
		for (size_t i = 0; i < m_SwapChainImageViews.size(); ++i)
		{
			// Specifies the attachments for each frame buffer
			std::array<VkImageView, 2> attachments = {
				m_SwapChainImageViews[i],			// ColorBuffer
				m_DepthImageView					// Depth Buffer
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			// Specifies the renderpass compatible with the framebuffer
			framebufferInfo.renderPass = m_RenderPass;
			// Specifies the attachments
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = m_SwapChainExtent.width;
			framebufferInfo.height = m_SwapChainExtent.height;
			// Layer number - More than 1 for 3D stereoscopic app
			framebufferInfo.layers = 1;

			// Creates the frambuffer
			VkResult result = vkCreateFramebuffer(m_LogicalDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create Framebuffer, Error Code: %d", result);
			}
		}
	}

	void VulkanWrapper::CreateCommandPool()
	{
		// Gets the queue families
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_PhysicalDevice);

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

	void VulkanWrapper::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		// Specifies the command pool
		allocInfo.commandPool = m_CommandPool;
		// Specifies if the command buffer is Primary or Secondary
		// Primary : Can be sent into a queue but can't be called by other command buffers
		// Secondary : Can be called from primary command buffers but can't be directly sent to a queue
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		// Specifies the number of command buffer to create
		allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

		// Creates the command buffer
		VkResult result = vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, m_CommandBuffers.data());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate command buffers, Error Code: %d", result);
		}
	}

	void VulkanWrapper::RecordCommandBuffer(VkCommandBuffer _CommandBuffer, uint32_t _ImageIndex)
	{
		// Command buffer begin recording infos
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		// Specifies the behavior of the command buffer (0 = None)
		beginInfo.flags = 0;
		// Only for secondary command buffers
		beginInfo.pInheritanceInfo = nullptr;

		// Start the command buffer
		VkResult result = vkBeginCommandBuffer(_CommandBuffer, &beginInfo);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to begin recording command buffer, Error Code: %d", result);
		}
		
		// Render pass infos
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		// Specifies render pass and attachments (FrameBuffer)
		renderPassInfo.renderPass = m_RenderPass;
		renderPassInfo.framebuffer = m_SwapChainFramebuffers[_ImageIndex];
		// Specifies render area
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChainExtent;

		// Creates the clear values
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { {0.2f, 0.2f, 1.f, 1.f} };	// Clear color
		clearValues[1].depthStencil = { 1.f, 0 };			// Clear depth

		// Specifies the clear values
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		// Starts the render pass
		// Last parameter: declares the origin of the commands here from a primary command buffer
		// VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFER -> for secondary command buffer
		vkCmdBeginRenderPass(_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Bind the pipeline (Second parameter specifies that this is a graphic pipeline and not a compute pipeline)
		vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

		// Defines the area where the image will be rendered
		VkViewport viewport{};
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = static_cast<float>(m_SwapChainExtent.width);
		viewport.height = static_cast<float>(m_SwapChainExtent.height);
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		vkCmdSetViewport(_CommandBuffer, 0, 1, &viewport);

		// Describes a limit where the image can be draw (similar to the viewport)
		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChainExtent;

		vkCmdSetScissor(_CommandBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

		VkBuffer vertexBuffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };

		// Binds the vertex buffer
		vkCmdBindVertexBuffers(_CommandBuffer, 0, 1, vertexBuffers, offsets);

		// Binds the index buffer
		vkCmdBindIndexBuffer(_CommandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

		// Binds descriptor sets (UBO)
		vkCmdBindDescriptorSets(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[m_CurrentFrame], 0, nullptr);

		// Draws the vertex buffer with indices
		vkCmdDrawIndexed(_CommandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		// Finish the render pass
		vkCmdEndRenderPass(_CommandBuffer);

		// Stops recording commands
		result = vkEndCommandBuffer(_CommandBuffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to record command buffer, Error Code %d", result);
		}
	}

	VkCommandBuffer VulkanWrapper::BeginSingleTimeCommands()
	{
		// Command buffer allocation infos
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;

		// Allocates the command buffer
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, &commandBuffer);

		// Begin info specifing VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT which means the command buffer will be used for a single command
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		// Start recording the command
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanWrapper::EndSingleTimeCommands(VkCommandBuffer _CommandBuffer)
	{
		// Ends recording the command
		vkEndCommandBuffer(_CommandBuffer);

		// Submit infos
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_CommandBuffer;

		// Sumbit the queue
		vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

		// Wait for the actions to finish before destroying it
		vkQueueWaitIdle(m_GraphicsQueue);

		// Free and destroy the command buffer
		vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, 1, &_CommandBuffer);
	}

	void VulkanWrapper::DrawFrame(GLFWwindow* _Window)
	{
		// Will wait the end of the rendering of the current frame (if rendering) and reset the fence
		// fourth parameter specifies that we want to wait for all the fences to finish (here we have only one fence)
		// last parameter specifies the delay before canceling (UINT64_MAX = cancel delay can wait indefinitely)
		vkWaitForFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]);

		uint32_t imageIndex;
		// Acquires an image from the swap chain
		// third parameter is a delay before canceling if no image is available -> here it's at UINT64_MAX which disable the delay
		// fourth and fifith parameter is the semaphore or fence that will be trigered when an image is available
		// the last parameter is the index of the image in the swap chain
		VkResult result = vkAcquireNextImageKHR(m_LogicalDevice, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain(_Window);
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			DEBUG_ERROR("Failed to acquire swap chain image, Error Code: %d", result);
		}

		// Updates the uniform buffer
		UpdateUniformBuffer(m_CurrentFrame);

		vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);
		RecordCommandBuffer(m_CommandBuffers[m_CurrentFrame], imageIndex);

		// Submit informations
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		// Here we reference the waiting semaphore and waiting stage
		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		// Specifies to vulkan to wait for m_ImageAvailableSemaphores at VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT to start rendering
		// We do not want to write the colors without an image so when an image is available vulkan can write in the image
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		// Specifies the command buffer(s) that will be executed
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame];

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };

		// Specifies the semaphores that will indicates if the frame finished rendering
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		// Sumbit the command buffer(s) for execution to the graphic queue
		// The last parameter is the fence that will signal when the frame finished rendering
		result = vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to submit draw command buffer, Error Code %d", result);
		}

		// Presentation informations
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		
		// Specifies the semaphore to wait for in order to present an image
		// Here we do not want to present an image it is not finished rendering
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain };

		// Reference our swap chain that will present the images
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		// Present the image with the presentation queue
		result = vkQueuePresentKHR(m_PresentQueue, &presentInfo);

		// Checks if the swap chain is no more compatible or inadequate for presentation or if the window has been resized
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized)
		{
			// Recreates the swap chain
			m_FramebufferResized = false;
			RecreateSwapChain(_Window);
		}
		else if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to present swap chain image, Error Code: %d", result);
		}

		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanWrapper::CreateSyncObjects()
	{
		m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		// Semaphore create info - for GPU/ GPU synchronization
		VkSemaphoreCreateInfo smaphoreInfo{};
		smaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		// Fence create info - for CPU / GPU synchronization
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// Specifies that the fence is created as signaled to avoid an infinite wait at the start of the draw
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		// Loop for all MAX_FRAMES_IIN_FLIGHT
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			// Creates Image available semaphores
			VkResult result = vkCreateSemaphore(m_LogicalDevice, &smaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create semaphore, Error Code: %d", result)
			}

			// Creates Render Finish semaphores
			result = vkCreateSemaphore(m_LogicalDevice, &smaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create semaphore, Error Code: %d", result)
			}

			// Creates In Flight fences
			result = vkCreateFence(m_LogicalDevice, &fenceInfo, nullptr, &m_InFlightFences[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create fence, Error Code: %d", result)
			}
		}
	}

	void VulkanWrapper::FrameBufferResizeCallback(GLFWwindow* _Window, int _Width, int _Height)
	{
		// Gets the custom pointer and cast it to our class
		VulkanWrapper* vkWrapper = reinterpret_cast<VulkanWrapper*>(glfwGetWindowUserPointer(_Window));

		vkWrapper->m_FramebufferResized = true;
	}

	void VulkanWrapper::CreateBuffer(VkDeviceSize _Size, VkBufferUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory)
	{
		// Buffer infos
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _Size;
		bufferInfo.usage = _Usage;
		// Same as for the image view we can choose to share the buffer between multiple queues or keep it for one which is the case here
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		// Creates the buffer
		VkResult result = vkCreateBuffer(m_LogicalDevice, &bufferInfo, nullptr, &_Buffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create vertex buffer, Error Code %d", result);
		}

		// Get the memory requirements to allocate the memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_LogicalDevice, _Buffer, &memRequirements);

		// Memory allocation infos
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, _Properties);

		// Allocates the memory
		result = vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &_BufferMemory);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate vertex buffer memory, Error Code: %d", result);
		}

		// Bind the buffer with allocated emplacement
		vkBindBufferMemory(m_LogicalDevice, _Buffer, _BufferMemory, 0);
	}

	void VulkanWrapper::CreateVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Creates a transfer buffer
		// VK_BUFFER_USAGE_TRANSFER_SRC_BIT specifies that the buffer can be used as a source buffer and be copied in the memory
		// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT specifies that the memory is the same CPU and GPU side so easily accessible by both but not optimized
		// This is why we are using staging buffer and we copy it in the VRAM
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Maps the data into the GPU Memory and copy the data into the memory
		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Creates the vertex buffer
		// VK_BUFFER_USAGE_TRANSFER_DST_BIT specifies that the buffer can only receive data from memory of the GPU and that it is a VERTEX_BUFFER
		// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT specifies that the memory is only for GPU and optimized for GPU
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		// Copies the data from the staging buffer to the vertex buffer
		CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

		// Destroys the stagging buffer and free the memory
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
	}

	uint32_t VulkanWrapper::FindMemoryType(uint32_t _TypeFilter, VkMemoryPropertyFlags _Properties)
	{
		// Gets all memory properties from the device
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

		// Loop going through all memory types
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			// _TypeFilter & (1 << i) checks if the i byte is equal to the one in _TypeFilter 
			// if its the case it means that the memory type correspond to the _TypeFilter
			// The second condition checks if all _Properties are present in memProperties.memoryTypes[i].propertyFlags
			if (_TypeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & _Properties) == _Properties)
			{
				return i;
			}
		}

		DEBUG_ERROR("Failed to find suitable memory type");
		return 0;
	}

	void VulkanWrapper::CopyBuffer(VkBuffer _SourceBuffer, VkBuffer _DestinationBuffer, VkDeviceSize _Size)
	{
		// Creates a temporary command buffer for transfer
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		// Buffer Copy infos
		VkBufferCopy copyRegion{};
		copyRegion.size = _Size;

		// Copies a buffer from a source buffer to a destination one
		vkCmdCopyBuffer(commandBuffer, _SourceBuffer, _DestinationBuffer, 1, &copyRegion);

		// Ends and destroys the buffer
		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanWrapper::CreateIndexBuffer()
	{
		// Same idea as the vertex buffer so please check the CreateVertexBuffer for more information

		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Creates a transfer buffer
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Maps the data into the GPU Memory and copy the data into the memory
		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Creates the index buffer
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

		// Copies the data from the staging buffer to the index buffer
		CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);

		// Destroys the stagging buffer and free the memory
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);

	}

	void VulkanWrapper::CreateDescriptorSetLayout()
	{
		// Descriptor Set layout binding for ubo
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		// Describes which stages can access the UBO can also be VK_SHADER_STAGE_ALL_GRAPHICS
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		// Descriptor Set layout binding for sampler
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		// Describes all the bindings possibles
		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		// Layou infos
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		// Nbr of bindings
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());;
		// Bindings data
		layoutInfo.pBindings = bindings.data();

		// Creates the Descriptor set layout
		VkResult result = vkCreateDescriptorSetLayout(m_LogicalDevice, &layoutInfo, nullptr, &m_DescriptorSetLayout);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create descriptor set layout, Error Code: %d", result);
		}
	}

	void VulkanWrapper::CreateUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformMVP);

		m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		m_UniformBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
		m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

		// Creates a single uniform buffer for each frames
		// We do not use staging buffer because the data will be updated every frame
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			// Creates a Uniform buffer
			CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBufferMemory[i]);
			// Maps the Memory
			vkMapMemory(m_LogicalDevice, m_UniformBufferMemory[i], 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
		}
	}

	void VulkanWrapper::UpdateUniformBuffer(uint32_t _CurrentImage)
	{
		// Creates a MVP
		UniformMVP mvp{};

		Math::Vector3 pos = Math::Vector3::zero;
		Math::Vector3 rot = Math::Vector3(Math::Utils::DegToRad(90.f), 0.f, iRandomRotation += 0.0005f);
		Math::Vector3 scale = Math::Vector3::one;

		// Updates the MVP
		mvp.model = Math::Matrix4::TRS(pos, rot, scale).Transpose();
		mvp.view = Math::Matrix4::ViewMatrix(Math::Vector3(-0.3f, 0.f, -2.f), Math::Vector3::zero, Math::Vector3::up).Transpose();
		mvp.projection = Math::Matrix4::ProjectionPerspectiveMatrix(0.01f, 100.f, (float)(m_SwapChainExtent.width / m_SwapChainExtent.height), 45.f).Transpose();

		// Copies the data into the buffer
		memcpy(m_UniformBuffersMapped[_CurrentImage], &mvp, sizeof(mvp));
	}

	void VulkanWrapper::CreateDescriptorPool()
	{
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
		VkResult result = vkCreateDescriptorPool(m_LogicalDevice, &poolInfo, nullptr, &m_DescriptorPool);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create descriptor pool, Error Code: %d", result);
		}
	}

	void VulkanWrapper::CreateDescriptorSets()
	{
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);

		// Allocation infos
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		// Size and data of all the Descriptor set layout
		allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

		// Allocates memory for all descriptor sets
		VkResult result = vkAllocateDescriptorSets(m_LogicalDevice, &allocInfo, m_DescriptorSets.data());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate descriptor sets, Error Code: %d", result);
		}

		// Specifies parameters for all descriptors
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			// UBO info
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_UniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformMVP);

			// Image info
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = m_TextureImageView;
			imageInfo.sampler = m_TextureSampler;

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			// Descriptor set to update
			descriptorWrites[0].dstSet = m_DescriptorSets[i];
			// Binding to update
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			// Update sampler
			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = m_DescriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			// Updates the descriptor sets
			vkUpdateDescriptorSets(m_LogicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}

	VkImageView VulkanWrapper::CreateImageView(VkImage _Image, VkFormat _Format, VkImageAspectFlags _AspectFlags)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _Image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = _Format;
		viewInfo.subresourceRange.aspectMask = _AspectFlags;
		// Mipmap levels
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		// baseArrayLayer - layerCount is the number of layers an image possess - only usefull for 3D stereoscopic applications
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		// Creates the image view
		VkImageView imageView;
		VkResult result = vkCreateImageView(m_LogicalDevice, &viewInfo, nullptr, &imageView);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture image view, Error Code: %d", result);
			return nullptr;
		}

		return imageView;
	}

	void VulkanWrapper::CreateTextureImage()
	{
		// Loads a texture with STB IMAGE
		int textWidth, textHeight, textChannels;
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* texture = stbi_load(TEXTURE_PATH.c_str(), &textWidth, &textHeight, &textChannels, STBI_rgb_alpha);

		VkDeviceSize imageSize = textWidth * textHeight * 4;

		if (!texture)
		{
			DEBUG_ERROR("Failed to load texture");
			return;
		}

		// Creates transfer buffers
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Creates buffer that can transfer memory from CPU to GPU
		CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Map the data into the buffer
		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, texture, static_cast<size_t>(imageSize));
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Free the texture on the CPU
		stbi_image_free(texture);

		// Creates the image object
		CreateImage(static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);

		// Transition the image layout a first time
		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy the texture in the good buffer
		CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight));

		// Transition the image layout a second time
		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Destroys the transfer buffer
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
	}

	void VulkanWrapper::CreateImage(uint32_t _Width, uint32_t _Height, VkFormat _Format, VkImageTiling _Tiling, VkImageUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkImage& _Image, VkDeviceMemory& _ImageMemory)
	{
		// Omage infos
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		// Type of texture
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = _Width;
		imageInfo.extent.height = _Height;
		// Depth is 1 because it's a 2D texture
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = _Format;
		// How pixels are organized in the memory
		imageInfo.tiling = _Tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = _Usage;
		// For multi sampling
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		// Defines if the image can be used by multiple queues
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		// Creates the image
		VkResult result = vkCreateImage(m_LogicalDevice, &imageInfo, nullptr, &_Image);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create image, Error Code: %d", result);
		}

		// Gets the memory requirements for the texture
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_LogicalDevice, _Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		// Allocates the memory
		result = vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &_ImageMemory);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Filed to allocate image memory, Error Code: %d", result);
		}

		// Bind the memory with our image
		vkBindImageMemory(m_LogicalDevice, _Image, _ImageMemory, 0);
	}

	void VulkanWrapper::TransitionImageLayout(VkImage _Image, VkFormat _Format, VkImageLayout _OldLayout, VkImageLayout _NewLayout)
	{
		// Starts recording command buffer
 		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		// Creates a memory barrier
		// Is here to garanty that we will not modify the memory while the GPU is using it or already writing in it
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;\
		// Specifies the old layout and the new layout
		barrier.oldLayout = _OldLayout;
		barrier.newLayout = _NewLayout;
		// Used to change queue families
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		// Base data of the image
		barrier.image = _Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		// Default values
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = 0;

		// Creates src and dst stages that we have to defined
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		// For depth buffer
		if (_NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(_Format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}

		// Handles some transitions
		if (_OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (_OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (_OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			// Also for depth buffer
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			DEBUG_ERROR("Unsupported layout transition !");
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		// Ends recording the command buffer
		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanWrapper::CopyBufferToImage(VkBuffer _Buffer, VkImage _Image, uint32_t _Width, uint32_t _Height)
	{
		// Records command buffer
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		// Creates the region to copy
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { _Width, _Height, 1 };

		// Copies the buffer to the image
		vkCmdCopyBufferToImage(commandBuffer, _Buffer, _Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		// Ends recording the command buffer
		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanWrapper::CreateTextureImageView()
	{
		// Uses the create image view function
		m_TextureImageView = CreateImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void VulkanWrapper::CreateTextureSampler()
	{
		// Sampler infos
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		// Same as OpenGL
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16.0f;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		// VK_FALSE = coords are between 0 and width or height and VK_TRUE coords are between 0 and 1
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		// Mip maps infos
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		// Creates the sampler
		VkResult result = vkCreateSampler(m_LogicalDevice, &samplerInfo, nullptr, &m_TextureSampler);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture sampler, Error Code: %d", result);
			return;
		}
	}

	void VulkanWrapper::CreateDepthRessources()
	{
		// Check for the best depth format available
		VkFormat depthFormat = FindDepthFormat();

		// Creates an image of the size of our rendering viewport (swap chain) - Creates also the image view to have access to the depth buffer
		CreateImage(m_SwapChainExtent.width, m_SwapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_DepthImage, m_DepthImageMemory);
		m_DepthImageView = CreateImageView(m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

		// Transition the layout
		TransitionImageLayout(m_DepthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	VkFormat VulkanWrapper::FindSupportedFormat(const std::vector<VkFormat>& _Candidates, VkImageTiling _Tiling, VkFormatFeatureFlags _Features)
	{
		// Loop through all candidates
		for (VkFormat format : _Candidates)
		{
			// Gets the formats properties in our device
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);

			// Checks the type of _Tiling and if the features required are supported
			if (_Tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & _Features) == _Features)
			{
				return format;
			}
			else if (_Tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & _Features) == _Features)
			{
				return format;
			}
		}

		DEBUG_ERROR("Failed to find supported format!");

		VkFormat errFormat;
		return errFormat;
	}

	VkFormat VulkanWrapper::FindDepthFormat()
	{
		// Checks a suported format for depth texture
		return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	bool VulkanWrapper::HasStencilComponent(VkFormat _Format)
	{
		return _Format == VK_FORMAT_D32_SFLOAT_S8_UINT || _Format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void VulkanWrapper::LoadModel()
	{
		// Sets up the variables for reading
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, error;

		// Loads the model
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, MODEL_PATH.c_str()))
		{
			DEBUG_ERROR("Failed to load model!");
		}

		for (const tinyobj::shape_t& shape : shapes)
		{
			for (const tinyobj::index_t& index : shape.mesh.indices)
			{
				// Creates each vertex
				Vertex vertex{};

				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.textCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.f, 1.f, 1.f };

				// Adds it to all vertices
				vertices.push_back(vertex);

				// Adds the index
				indices.push_back(static_cast<uint32_t>(indices.size()));
			}
		}
	}
}