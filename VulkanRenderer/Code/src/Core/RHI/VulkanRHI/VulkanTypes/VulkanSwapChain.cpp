#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"

#include <set>

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanImage.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanImageView.h"

namespace Core
{
	const bool VulkanSwapChain::CheckDeviceExtensionSupport(VulkanDevice _Device, const std::vector<const char*> _DeviceExtensions)
	{
		// Gets the number of extensions supported by our device
		uint32_t extensionsNbr;
		vkEnumerateDeviceExtensionProperties(_Device.GetPhysicalDevice(), nullptr, &extensionsNbr, nullptr);

		// Gets all the extensions supported by our device
		std::vector<VkExtensionProperties> availableExtensions(extensionsNbr);
		vkEnumerateDeviceExtensionProperties(_Device.GetPhysicalDevice(), nullptr, &extensionsNbr, availableExtensions.data());

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

	SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupport(VulkanDevice _Device)
	{
		SwapChainSupportDetails details;

		// Checks the surfaces capabilities (min / max frames in the swap chain, min / max - height / width of the images)
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_Device.GetPhysicalDevice(), _Device.GetSurface(), &details.capabilities);

		// Checks the number of format available (pixels format / colors)
		uint32_t formatNbr;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_Device.GetPhysicalDevice(), _Device.GetSurface(), &formatNbr, nullptr);

		// References the formats if availables
		if (formatNbr != 0)
		{
			details.formats.resize(formatNbr);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_Device.GetPhysicalDevice(), _Device.GetSurface(), &formatNbr, details.formats.data());
		}

		// Checks the number of presentations modes (FIFO / Mailbox...)
		uint32_t presentModeNbr;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_Device.GetPhysicalDevice(), _Device.GetSurface(), &presentModeNbr, nullptr);

		// References the presentations modes if availables
		if (presentModeNbr != 0)
		{
			details.presentModes.resize(presentModeNbr);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_Device.GetPhysicalDevice(), _Device.GetSurface(), &presentModeNbr, details.presentModes.data());
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

	VkExtent2D VulkanSwapChain::ChooseSwapExtent(Window* _Window, const VkSurfaceCapabilitiesKHR& _Capabilities)
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
			glfwGetFramebufferSize(_Window->GetWindowPointer(), &width, &height);

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

	void VulkanSwapChain::CreateSwapChainFramebuffers()
	{
		m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

		// Loop going through every image view of the swap chain
		for (size_t i = 0; i < m_SwapChainImageViews.size(); ++i)
		{
		}
	}

	void VulkanSwapChain::CreateSwapChainImageViews()
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); ++i)
		{
			VulkanImageView image;
			// Creates an image view for each image of the swap chain
			m_SwapChainImageViews[i] = image.CreateImageView(m_SwapChainImages[i], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		return;
	}

	const RHI_RESULT VulkanSwapChain::CreateSwapChain(Window* _Window, IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Gets all the capabilities and information the swapchain can support
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);

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
		createInfo.surface = device.GetSurface();
		createInfo.minImageCount = imageNbr;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		// imageArrayLayers is the number of layers an image possess - only usefull for 3D stereoscopic applications
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// Gets all queue families to setup up sharing mode of the images in the swap chain
		QueueFamilyIndices indices = VulkanQueue::FindQueueFamilies(device);
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
		VkResult result = vkCreateSwapchainKHR(device.GetLogicalDevice(), &createInfo, nullptr, &m_SwapChain);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create swap chain, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		// Gets all the images of the swap chain
		vkGetSwapchainImagesKHR(device.GetLogicalDevice(), m_SwapChain, &imageNbr, nullptr);
		m_SwapChainImages.resize(imageNbr);
		vkGetSwapchainImagesKHR(device.GetLogicalDevice(), m_SwapChain, &imageNbr, m_SwapChainImages.data());

		return RHI_SUCCESS;
	}

	const RHI_RESULT VulkanSwapChain::RecreateSwapChain(Window* _Window, IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Gets the current size of the window
		int width = 0, height = 0;
		glfwGetFramebufferSize(_Window->GetWindowPointer(), &width, &height);

		// Loop executed when the window is minimized (when width and height = 0)
		while (width == 0 || height == 0)
		{
			// Updates the size to get out when not minimized
			glfwGetFramebufferSize(_Window->GetWindowPointer(), &width, &height);

			// Pause all events
			glfwWaitEvents();
		}

		// Waits the end of the utilisation of all resources to not access them when used
		vkDeviceWaitIdle(device.GetLogicalDevice());

		// Destroys the swap chain
		DestroySwapChain(_Device);

		// Recreates the swap chain
		RHI_RESULT result = CreateSwapChain(_Window, _Device);

		//CreateSwapChainImageViews();
		//CreateDepthRessources();
		//CreateSwapChainFramebuffers();

		return result;
	}

	const RHI_RESULT VulkanSwapChain::DestroySwapChain(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Destroys data linked to the swap chain
		//vkDestroyImageView(_LogicalDevice, m_DepthImageView, nullptr);
		//vkDestroyImage(_LogicalDevice, m_DepthImage, nullptr);
		//vkFreeMemory(_LogicalDevice, m_DepthImageMemory, nullptr);

		for (auto framebuffer : m_SwapChainFramebuffers)
		{
			vkDestroyFramebuffer(device.GetLogicalDevice(), framebuffer, nullptr);
		}

		for (VkImageView imageView : m_SwapChainImageViews)
		{
			vkDestroyImageView(device.GetLogicalDevice(), imageView, nullptr);
		}

		// Destroys the swap chain
		vkDestroySwapchainKHR(device.GetLogicalDevice(), m_SwapChain, nullptr);

		return RHI_SUCCESS;
	}
}