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
	VulkanWrapper::VulkanWrapper()
	{
	}

	VulkanWrapper::~VulkanWrapper()
	{
		m_SwapChainImages.shrink_to_fit();
		m_SwapChainImageViews.shrink_to_fit();
		m_SwapChainFramebuffers.shrink_to_fit();
		m_CommandBuffers.shrink_to_fit();
		m_ImageAvailableSemaphores.shrink_to_fit();
		m_RenderFinishedSemaphores.shrink_to_fit();
		m_InFlightFences.shrink_to_fit();
		m_UniformBuffers.shrink_to_fit();
		m_UniformBufferMemory.shrink_to_fit();
		m_UniformBuffersMapped.shrink_to_fit();
		m_DescriptorSets.shrink_to_fit();
	}

	const bool VulkanWrapper::Initialize(GLFWwindow* _Window)
	{
		

		if (!CreateVulkanInstance())
			return false;

		glfwSetWindowUserPointer(_Window, this);
		glfwSetFramebufferSizeCallback(_Window, FrameBufferResizeCallback);

		SetupDebugMessenger();
		CreateSurface(_Window);
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapChain(_Window);
		CreateImageViews();
		CreateRenderPass();
		CreateDescriptorSetLayout();
		CreateGraphicsPipeline();
		CreateCommandPool();
		CreateDepthRessources();
		CreateFramebuffers();
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
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
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
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(_Device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(_Device, &deviceFeatures);

		QueueFamilyIndices indices = FindQueueFamilies(_Device);

		bool areExtensionsSupported = CheckDeviceExtensionSupport(_Device);

		bool isSwapChainAdequate = false;

		if (areExtensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_Device);
			isSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

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

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyNbr);
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &queueFamilyNbr, queueFamilies.data());

		int index = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = index;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_Device, index, m_Surface, &presentSupport);

			if (presentSupport)
			{
				indices.presentFamily = index;
			}

			if (indices.graphicsFamily.has_value() && indices.presentFamily.has_value())
			{
				break;
			}

			++index;
		}

		indices.isComplete = indices.graphicsFamily.has_value() && indices.presentFamily.has_value();

		return indices;
	}

	void VulkanWrapper::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
		float queuePriority = 1.0f;

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		VkResult result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to created logical device, Error Code: %d", result);
			return;
		}

		vkGetDeviceQueue(m_LogicalDevice, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, indices.presentFamily.value(), 0, &m_PresentQueue);

		return;
	}

	void VulkanWrapper::CreateSurface(GLFWwindow* _Window)
	{
		VkResult result = glfwCreateWindowSurface(m_VulkanInstance, _Window, nullptr, &m_Surface);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create window surface, Error Code: %d", result);
		}
	}

	const bool VulkanWrapper::CheckDeviceExtensionSupport(VkPhysicalDevice _Device)
	{
		uint32_t extensionsNbr;
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &extensionsNbr, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionsNbr);
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &extensionsNbr, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

		for (const VkExtensionProperties& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	SwapChainSupportDetails VulkanWrapper::QuerySwapChainSupport(VkPhysicalDevice _Device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_Device, m_Surface, &details.capabilities);

		uint32_t formatNbr;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, m_Surface, &formatNbr, nullptr);

		if (formatNbr != 0)
		{
			details.formats.resize(formatNbr);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, m_Surface, &formatNbr, details.formats.data());
		}

		uint32_t presentModeNbr;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, m_Surface, &presentModeNbr, nullptr);

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
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return format;
			}
		}

		return _AvailableFormats[0];
	}

	VkPresentModeKHR VulkanWrapper::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _AvailablePresentModes)
	{
		for (const VkPresentModeKHR& presentMode : _AvailablePresentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return presentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanWrapper::ChooseSwapExtent(GLFWwindow* _Window, const VkSurfaceCapabilitiesKHR& _Capabilities)
	{
		if (_Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return _Capabilities.currentExtent;
		}
		else
		{
			int width, height;
			glfwGetFramebufferSize(_Window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, _Capabilities.minImageExtent.width, _Capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, _Capabilities.minImageExtent.height, _Capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void VulkanWrapper::CreateSwapChain(GLFWwindow* _Window)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_PhysicalDevice);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);

		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);

		VkExtent2D extent = ChooseSwapExtent(_Window, swapChainSupport.capabilities);

		uint32_t imageNbr = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageNbr > swapChainSupport.capabilities.maxImageCount)
		{
			imageNbr = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = imageNbr;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		m_SwapChainImageFormat = surfaceFormat.format;
		m_SwapChainExtent = extent;

		VkResult result = vkCreateSwapchainKHR(m_LogicalDevice, &createInfo, nullptr, &m_SwapChain);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create swap chain, Error Code: %d", result);
			return;
		}

		vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageNbr, nullptr);
		m_SwapChainImages.resize(imageNbr);
		vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageNbr, m_SwapChainImages.data());

		return;
	}

	void VulkanWrapper::RecreateSwapChain(GLFWwindow* _Window)
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(_Window, &width, &height);

		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(_Window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_LogicalDevice);

		CleanSwapChain();

		CreateSwapChain(_Window);
		CreateImageViews();
		CreateDepthRessources();
		CreateFramebuffers();
	}

	void VulkanWrapper::CleanSwapChain()
	{
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

		vkDestroySwapchainKHR(m_LogicalDevice, m_SwapChain, nullptr);

	}

	void VulkanWrapper::CreateImageViews()
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); ++i)
		{
			m_SwapChainImageViews[i] = CreateImageView(m_SwapChainImages[i], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		return;
	}

	void VulkanWrapper::CreateGraphicsPipeline()
	{
		std::vector<char> verShader = ReadShader("Assets/Shaders/vert.spv");
		std::vector<char> fragShader = ReadShader("Assets/Shaders/frag.spv");

		VkShaderModule vertShaderModule = CreateShaderModule(verShader);
		VkShaderModule fragShaderModule = CreateShaderModule(fragShader);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkVertexInputBindingDescription bindingDescription = Core::Vertex::GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = Core::Vertex::GetAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.f;
		rasterizer.depthBiasClamp = 0.f;
		rasterizer.depthBiasSlopeFactor = 0.f;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.f;
		depthStencil.maxDepthBounds = 1.f;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

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

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		VkResult result = vkCreatePipelineLayout(m_LogicalDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create pipeline layout, Error Code: %d", result);
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		result = vkCreateGraphicsPipelines(m_LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create graphics pipeline, Error Code: %d", result);
		}

		vkDestroyShaderModule(m_LogicalDevice, fragShaderModule, nullptr);
		vkDestroyShaderModule(m_LogicalDevice, vertShaderModule, nullptr);
	}

	std::vector<char> VulkanWrapper::ReadShader(const std::filesystem::path& _FilePath)
	{
		if (_FilePath.extension() != ".spv")
		{
			DEBUG_ERROR("Wrong shader file extension");
			std::vector<char> err(0);
			return err;
		}

		std::ifstream file(_FilePath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			DEBUG_ERROR("Failed to open shader file");
			std::vector<char> err(0);
			return err;
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkShaderModule VulkanWrapper::CreateShaderModule(const std::vector<char>& _ShaderSourceCode)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = _ShaderSourceCode.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(_ShaderSourceCode.data());

		VkShaderModule shaderModule;

		VkResult result = vkCreateShaderModule(m_LogicalDevice, &createInfo, nullptr, &shaderModule);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create shader module, Error Code: %d", result);
		}

		return shaderModule;
	}

	void VulkanWrapper::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_SwapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = FindDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
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

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VkResult result = vkCreateRenderPass(m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create render pass, Error Code: %d", result);
		}
	}

	void VulkanWrapper::CreateFramebuffers()
	{
		m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

		for (size_t i = 0; i < m_SwapChainImageViews.size(); ++i)
		{
			std::array<VkImageView, 2> attachments = {
				m_SwapChainImageViews[i],
				m_DepthImageView
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = m_SwapChainExtent.width;
			framebufferInfo.height = m_SwapChainExtent.height;
			framebufferInfo.layers = 1;

			VkResult result = vkCreateFramebuffer(m_LogicalDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create Framebuffer, Error Code: %d", result);
			}
		}
	}

	void VulkanWrapper::CreateCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_PhysicalDevice);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

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
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t) m_CommandBuffers.size();

		VkResult result = vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, m_CommandBuffers.data());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate command buffers, Error Code: %d", result);
		}
	}

	void VulkanWrapper::RecordCommandBuffer(VkCommandBuffer _CommandBuffer, uint32_t _ImageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		VkResult result = vkBeginCommandBuffer(_CommandBuffer, &beginInfo);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to begin recording command buffer, Error Code: %d", result);
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_RenderPass;
		renderPassInfo.framebuffer = m_SwapChainFramebuffers[_ImageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChainExtent;

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {{0.f, 0.f, 0.f, 1.f}};
		clearValues[1].depthStencil = { 1.f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

		VkViewport viewport{};
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = static_cast<float>(m_SwapChainExtent.width);
		viewport.height = static_cast<float>(m_SwapChainExtent.height);
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		vkCmdSetViewport(_CommandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChainExtent;

		vkCmdSetScissor(_CommandBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

		VkBuffer vertexBuffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(_CommandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(_CommandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[m_CurrentFrame], 0, nullptr);

		vkCmdDrawIndexed(_CommandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(_CommandBuffer);

		result = vkEndCommandBuffer(_CommandBuffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to record command buffer, Error Code %d", result);
		}
	}

	VkCommandBuffer VulkanWrapper::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanWrapper::EndSingleTimeCommands(VkCommandBuffer _CommandBuffer)
	{
		vkEndCommandBuffer(_CommandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_CommandBuffer;

		vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_GraphicsQueue);

		vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, 1, &_CommandBuffer);
	}

	void VulkanWrapper::DrawFrame(GLFWwindow* _Window)
	{
		vkWaitForFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
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

		UpdateUniformBuffer(m_CurrentFrame);

		vkResetFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]);

		vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);
		RecordCommandBuffer(m_CommandBuffers[m_CurrentFrame], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame];

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		result = vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to submit draw command buffer, Error Code %d", result);
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain };

		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		result = vkQueuePresentKHR(m_PresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized)
		{
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

		VkSemaphoreCreateInfo smaphoreInfo{};
		smaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			VkResult result = vkCreateSemaphore(m_LogicalDevice, &smaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create semaphore, Error Code: %d", result)
			}

			result = vkCreateSemaphore(m_LogicalDevice, &smaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create semaphore, Error Code: %d", result)
			}

			result = vkCreateFence(m_LogicalDevice, &fenceInfo, nullptr, &m_InFlightFences[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create fence, Error Code: %d", result)
			}
		}
	}

	void VulkanWrapper::FrameBufferResizeCallback(GLFWwindow* _Window, int _Width, int _Height)
	{
		VulkanWrapper* vkWrapper = reinterpret_cast<VulkanWrapper*>(glfwGetWindowUserPointer(_Window));
		vkWrapper->m_FramebufferResized = true;
	}

	void VulkanWrapper::CreateBuffer(VkDeviceSize _Size, VkBufferUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _Size;
		bufferInfo.usage = _Usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult result = vkCreateBuffer(m_LogicalDevice, &bufferInfo, nullptr, &_Buffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create vertex buffer, Error Code %d", result);
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_LogicalDevice, _Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, _Properties);

		result = vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &_BufferMemory);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate vertex buffer memory, Error Code: %d", result);
		}

		vkBindBufferMemory(m_LogicalDevice, _Buffer, _BufferMemory, 0);
	}

	void VulkanWrapper::CreateVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
	}

	uint32_t VulkanWrapper::FindMemoryType(uint32_t _TypeFilter, VkMemoryPropertyFlags _Properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);
		
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
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
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = _Size;
		vkCmdCopyBuffer(commandBuffer, _SourceBuffer, _DestinationBuffer, 1, &copyRegion);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanWrapper::CreateIndexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

		CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);

		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);

	}

	void VulkanWrapper::CreateDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());;
		layoutInfo.pBindings = bindings.data();

		VkResult result = vkCreateDescriptorSetLayout(m_LogicalDevice, &layoutInfo, nullptr, &m_DescriptorSetLayout);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create descriptor set layout, Error Code: %d", result);
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
	}

	void VulkanWrapper::CreateUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformMVP);

		m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		m_UniformBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
		m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBufferMemory[i]);
			vkMapMemory(m_LogicalDevice, m_UniformBufferMemory[i], 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
		}
	}

	void VulkanWrapper::UpdateUniformBuffer(uint32_t _CurrentImage)
	{
		UniformMVP mvp{};

		Math::Vector3 pos = Math::Vector3::zero;
		Math::Vector3 rot = Math::Vector3(Math::Utils::DegToRad(90.f), 0.f, iRandomRotation += 0.0005f);
		Math::Vector3 scale = Math::Vector3::one;

		mvp.model = Math::Matrix4::TRS(pos, rot, scale);
		mvp.view = Math::Matrix4::ViewMatrix(Math::Vector3(-0.3f, 0.f, -2.f), Math::Vector3::zero, Math::Vector3::up);
		mvp.projection = Math::Matrix4::ProjectionPerspectiveMatrix(0.01f, 100.f, (float)(m_SwapChainExtent.width / m_SwapChainExtent.height), 45.f);

		memcpy(m_UniformBuffersMapped[_CurrentImage], &mvp, sizeof(mvp));
	}

	void VulkanWrapper::CreateDescriptorPool()
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

		VkResult result = vkCreateDescriptorPool(m_LogicalDevice, &poolInfo, nullptr, &m_DescriptorPool);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create descriptor pool, Error Code: %d", result);
		}
	}

	void VulkanWrapper::CreateDescriptorSets()
	{
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);
		
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

		VkResult result = vkAllocateDescriptorSets(m_LogicalDevice, &allocInfo, m_DescriptorSets.data());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate descriptor sets, Error Code: %d", result);
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_UniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformMVP);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = m_TextureImageView;
			imageInfo.sampler = m_TextureSampler;

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = m_DescriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = m_DescriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

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
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

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
		int textWidth, textHeight, textChannels;
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* texture = stbi_load(TEXTURE_PATH.c_str(), &textWidth, &textHeight, &textChannels, STBI_rgb_alpha);

		VkDeviceSize imageSize = textWidth * textHeight * 4;

		if (!texture)
		{
			DEBUG_ERROR("Failed to load texture");
			return;
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, texture, static_cast<size_t>(imageSize));
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		stbi_image_free(texture);

		CreateImage(static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);
		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight));
		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
	}

	void VulkanWrapper::CreateImage(uint32_t _Width, uint32_t _Height, VkFormat _Format, VkImageTiling _Tiling, VkImageUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkImage& _Image, VkDeviceMemory& _ImageMemory)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = _Width;
		imageInfo.extent.height = _Height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = _Format;
		imageInfo.tiling = _Tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = _Usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		VkResult result = vkCreateImage(m_LogicalDevice, &imageInfo, nullptr, &_Image);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create image, Error Code: %d", result);
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_LogicalDevice, _Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		result = vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &_ImageMemory);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Filed to allocate image memory, Error Code: %d", result);
		}

		vkBindImageMemory(m_LogicalDevice, _Image, _ImageMemory, 0);
	}

	void VulkanWrapper::TransitionImageLayout(VkImage _Image, VkFormat _Format, VkImageLayout _OldLayout, VkImageLayout _NewLayout)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = _OldLayout;
		barrier.newLayout = _NewLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = _Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = 0;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (_NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(_Format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}

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

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanWrapper::CopyBufferToImage(VkBuffer _Buffer, VkImage _Image, uint32_t _Width, uint32_t _Height)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

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

		vkCmdCopyBufferToImage(commandBuffer, _Buffer, _Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		EndSingleTimeCommands(commandBuffer);
	}
	
	void VulkanWrapper::CreateTextureImageView()
	{
		m_TextureImageView = CreateImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void VulkanWrapper::CreateTextureSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		VkResult result = vkCreateSampler(m_LogicalDevice, &samplerInfo, nullptr, &m_TextureSampler);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture sampler, Error Code: %d", result);
			return;
		}
	}

	void VulkanWrapper::CreateDepthRessources()
	{
		VkFormat depthFormat = FindDepthFormat();

		CreateImage(m_SwapChainExtent.width, m_SwapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_DepthImage, m_DepthImageMemory);
		m_DepthImageView = CreateImageView(m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

		TransitionImageLayout(m_DepthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	VkFormat VulkanWrapper::FindSupportedFormat(const std::vector<VkFormat>& _Candidates, VkImageTiling _Tiling, VkFormatFeatureFlags _Features)
	{
		for (VkFormat format : _Candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);

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
	}

	VkFormat VulkanWrapper::FindDepthFormat()
	{
		return FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	bool VulkanWrapper::HasStencilComponent(VkFormat _Format)
	{
		return _Format == VK_FORMAT_D32_SFLOAT_S8_UINT || _Format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void VulkanWrapper::LoadModel()
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, error;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, MODEL_PATH.c_str()))
		{
			DEBUG_ERROR("Failed to load model!");
		}

		for (const tinyobj::shape_t& shape : shapes)
		{
			for (const tinyobj::index_t& index : shape.mesh.indices)
			{
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

				vertices.push_back(vertex);
				indices.push_back(indices.size());
			}
		}
	}
}