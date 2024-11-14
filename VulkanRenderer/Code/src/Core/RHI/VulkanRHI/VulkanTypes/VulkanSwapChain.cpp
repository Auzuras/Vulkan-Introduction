#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanImage.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanSemaphore.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanFence.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandBuffer.h"
#include "RHI/RHITypes/IPipeline.h"

#include <set>

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

	void VulkanSwapChain::AcquireNextImage(Window* _Window, IDevice* _Device, IPipeline* _Pipeline, unsigned int _Timeout, ISemaphore* _ImageAvailableSemaphore, unsigned int& _ImageIndex)
	{
		VulkanDevice device = *_Device->CastToVulkan();
		VulkanSemaphore semaphore = *_ImageAvailableSemaphore->CastToVulkan();

		// Acquires an image from the swap chain
		// third parameter is a delay before canceling if no image is available -> here it's at UINT64_MAX which disable the delay
		// fourth and fifith parameter is the semaphore or fence that will be trigered when an image is available
		// the last parameter is the index of the image in the swap chain
		VkResult result = vkAcquireNextImageKHR(device.GetLogicalDevice(), m_SwapChain, _Timeout, semaphore.GetSemaphore(), VK_NULL_HANDLE, &_ImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain(_Window, _Device, _Pipeline);
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			DEBUG_ERROR("Failed to acquire swap chain image, Error Code: %d", result);
		}
	}

	RHI_RESULT VulkanSwapChain::SubmitGraphicsQueue(IDevice* _Device, ICommandBuffer* _CommandBuffer, ISemaphore* _ImageAvailableSemaphore, ISemaphore* _RenderFinishSemaphore, IFence* _InFlightFence)
	{
		VulkanDevice device = *_Device->CastToVulkan();
		VkCommandBuffer commandbuffer = _CommandBuffer->CastToVulkan()->GetCommandBuffer();

		VulkanSemaphore imageAvailableSync = *_ImageAvailableSemaphore->CastToVulkan();
		VulkanSemaphore renderFinishSync = *_RenderFinishSemaphore->CastToVulkan();
		VulkanFence inFlightSync = *_InFlightFence->CastToVulkan();

		// Submit informations
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		// Here we reference the waiting semaphore and waiting stage
		VkSemaphore waitSemaphores[] = { imageAvailableSync.GetSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		// Specifies to vulkan to wait for m_ImageAvailableSemaphores at VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT to start rendering
		// We do not want to write the colors without an image so when an image is available vulkan can write in the image
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		// Specifies the command buffer(s) that will be executed
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandbuffer;

		VkSemaphore signalSemaphores[] = { renderFinishSync.GetSemaphore() };

		// Specifies the semaphores that will indicates if the frame finished rendering
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		// Sumbit the command buffer(s) for execution to the graphic queue
		// The last parameter is the fence that will signal when the frame finished rendering
		VkResult result = vkQueueSubmit(device.GetGraphicsQueue(), 1, &submitInfo, inFlightSync.GetType());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to submit draw command buffer, Error Code %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	RHI_RESULT VulkanSwapChain::SubmitPresentQueue(Window* _Window, IDevice* _Device, IPipeline* _Pipeline, ISemaphore* _RenderFinishSemaphore, unsigned int _ImageIndex)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		VulkanSemaphore renderFinishSync = *_RenderFinishSemaphore->CastToVulkan();

		VkSemaphore signalSemaphores[] = { renderFinishSync.GetSemaphore() };

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
		presentInfo.pImageIndices = &_ImageIndex;
		presentInfo.pResults = nullptr;

		// Present the image with the presentation queue
		VkResult result = vkQueuePresentKHR(device.GetPresentationQueue(), &presentInfo);

		// Checks if the swap chain is no more compatible or inadequate for presentation or if the window has been resized
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
		{
			// Recreates the swap chain
			framebufferResized = false;
			RecreateSwapChain(_Window, _Device, _Pipeline);
		}
		else if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to present swap chain image, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
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

	RHI_RESULT VulkanSwapChain::CreateSwapChainFramebuffers(IDevice* _Device, IPipeline* _Pipeline)
	{
		m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

		// Loop going through every image view of the swap chain
		for (size_t i = 0; i < m_SwapChainFramebuffers.size(); ++i)
		{
			m_SwapChainFramebuffers[i].CreateFramebuffer(_Device, _Pipeline, m_SwapChainExtent.width, m_SwapChainExtent.height, m_SwapChainImageViews[i].GetImageView(), m_DepthImageView.GetImageView());
		}

		return RHI_SUCCESS;
	}

	void VulkanSwapChain::CreateSwapChainImageViews(VkDevice _Device)
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); ++i)
		{
			// Creates an image view for each image of the swap chain
			VulkanImageView imageView;
			imageView.CreateImageView(_Device, m_SwapChainImages[i], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);

			m_SwapChainImageViews[i] = imageView;
		}

		return;
	}

	VulkanSwapChain::~VulkanSwapChain()
	{}

	const RHI_RESULT VulkanSwapChain::CreateSwapChain(Window* _Window, IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Gets all the capabilities and information the swapchain can support
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device.GetPhysicalDevice(), device.GetSurface());

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
		QueueFamilyIndices indices = VulkanQueue::FindQueueFamilies(device.GetPhysicalDevice(), device.GetSurface());
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

		CreateSwapChainImageViews(device.GetLogicalDevice());

		return RHI_SUCCESS;
	}

	const RHI_RESULT VulkanSwapChain::RecreateSwapChain(Window* _Window, IDevice* _Device, IPipeline* _Pipeline)
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

		CreateSwapChainImageViews(device.GetLogicalDevice());
		VulkanImage::CreateDepthRessources(_Device, m_SwapChainExtent.width, m_SwapChainExtent.height, &m_DepthImage, &m_DepthImageView, m_DepthImageMemory);
		CreateSwapChainFramebuffers(_Device, _Pipeline);

		return result;
	}

	const RHI_RESULT VulkanSwapChain::DestroySwapChain(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Destroys data linked to the swap chain
		vkDestroyImageView(device.GetLogicalDevice(), m_DepthImageView.GetImageView(), nullptr);
		vkDestroyImage(device.GetLogicalDevice(), m_DepthImage.GetImage(), nullptr);
		vkFreeMemory(device.GetLogicalDevice(), m_DepthImageMemory, nullptr);

		for (VulkanFramebuffer framebuffer : m_SwapChainFramebuffers)
		{
			framebuffer.DestroyFramebuffer(_Device);
		}

		for (VulkanImageView imageView : m_SwapChainImageViews)
		{
			imageView.DestroyImageView(device.GetLogicalDevice());
		}

		// Destroys the swap chain
		vkDestroySwapchainKHR(device.GetLogicalDevice(), m_SwapChain, nullptr);

		return RHI_SUCCESS;
	}
}