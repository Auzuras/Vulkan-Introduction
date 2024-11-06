#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"

#include <set>

namespace Core
{
	const bool VulkanSwapChain::CheckDeviceExtensionSupport(VkPhysicalDevice _Device, const std::vector<const char*> _DeviceExtensions)
	{
		// Gets the number of extensions supported by our device
		uint32_t extensionsNbr;
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &extensionsNbr, nullptr);

		// Gets all the extensions supported by our device
		std::vector<VkExtensionProperties> availableExtensions(extensionsNbr);
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &extensionsNbr, availableExtensions.data());

		std::set<std::string> requiredExtensions(_DeviceExtensions.begin(), _DeviceExtensions.end());

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

	SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupport(VkPhysicalDevice _Device, VkSurfaceKHR _Surface)
	{
		SwapChainSupportDetails details;

		// Checks the surfaces capabilities (min / max frames in the swap chain, min / max - height / width of the images)
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_Device, _Surface, &details.capabilities);

		// Checks the number of format available (pixels format / colors)
		uint32_t formatNbr;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, _Surface, &formatNbr, nullptr);

		// References the formats if availables
		if (formatNbr != 0)
		{
			details.formats.resize(formatNbr);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, _Surface, &formatNbr, details.formats.data());
		}

		// Checks the number of presentations modes (FIFO / Mailbox...)
		uint32_t presentModeNbr;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, _Surface, &presentModeNbr, nullptr);

		// References the presentations modes if availables
		if (presentModeNbr != 0)
		{
			details.presentModes.resize(presentModeNbr);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, _Surface, &presentModeNbr, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _AvailableFormats)
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

	VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _AvailablePresentModes)
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

	VkExtent2D VulkanSwapChain::ChooseSwapExtent(GLFWwindow* _Window, const VkSurfaceCapabilitiesKHR& _Capabilities)
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

	void VulkanSwapChain::CreateSwapChain(GLFWwindow* _Window, VkDevice _LogicalDevice, VkPhysicalDevice _Device, VkSurfaceKHR _Surface)
	{
		// Gets all the capabilities and information the swapchain can support
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_Device, _Surface);

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
		createInfo.surface = _Surface;
		createInfo.minImageCount = imageNbr;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		// imageArrayLayers is the number of layers an image possess - only usefull for 3D stereoscopic applications
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// Gets all queue families to setup up sharing mode of the images in the swap chain
		QueueFamilyIndices indices = VulkanRenderer::FindQueueFamilies(_Device, _Surface);
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
		VkResult result = vkCreateSwapchainKHR(_LogicalDevice, &createInfo, nullptr, &m_SwapChain);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create swap chain, Error Code: %d", result);
			return;
		}

		// Gets all the images of the swap chain
		vkGetSwapchainImagesKHR(_LogicalDevice, m_SwapChain, &imageNbr, nullptr);
		m_SwapChainImages.resize(imageNbr);
		vkGetSwapchainImagesKHR(_LogicalDevice, m_SwapChain, &imageNbr, m_SwapChainImages.data());

		return;
	}

	void VulkanSwapChain::RecreateSwapChain(GLFWwindow* _Window, VkDevice _LogicalDevice, VkPhysicalDevice _Device, VkSurfaceKHR _Surface)
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
		vkDeviceWaitIdle(_LogicalDevice);

		// Destroys the swap chain
		CleanSwapChain(_LogicalDevice);

		// Recreates the swap chain
		CreateSwapChain(_Window, _LogicalDevice, _Device, _Surface);
		//CreateSwapChainImageViews();
		//CreateDepthRessources();
		//CreateSwapChainFramebuffers();
	}

	void VulkanSwapChain::CleanSwapChain(VkDevice _LogicalDevice)
	{
		// Destroys data linked to the swap chain
		//vkDestroyImageView(_LogicalDevice, m_DepthImageView, nullptr);
		//vkDestroyImage(_LogicalDevice, m_DepthImage, nullptr);
		//vkFreeMemory(_LogicalDevice, m_DepthImageMemory, nullptr);

		for (auto framebuffer : m_SwapChainFramebuffers)
		{
			vkDestroyFramebuffer(_LogicalDevice, framebuffer, nullptr);
		}

		for (VkImageView imageView : m_SwapChainImageViews)
		{
			vkDestroyImageView(_LogicalDevice, imageView, nullptr);
		}

		// Destroys the swap chain
		vkDestroySwapchainKHR(_LogicalDevice, m_SwapChain, nullptr);
	}
}