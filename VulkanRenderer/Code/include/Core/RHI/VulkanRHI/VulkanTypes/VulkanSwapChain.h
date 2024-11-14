#pragma once

#include "RHI/RHITypes/ISwapChain.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanImage.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanImageView.h"

namespace Core
{
	class VulkanDevice;

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanSwapChain : public ISwapChain
	{
	private:

		VkSwapchainKHR m_SwapChain;

		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

		std::vector<VkImage> m_SwapChainImages;
		std::vector<VulkanImageView> m_SwapChainImageViews;

		std::vector<VkFramebuffer> m_SwapChainFramebuffers;

		VulkanImage m_DepthImage;
		VulkanImageView m_DepthImageView;
		VkDeviceMemory m_DepthImageMemory;

		/// <summary>
		/// Checks if the extensions needed for our program are supported by our GPU
		/// </summary>
		/// <param name="_Device">: Logical device which we check extensions support </param>
		/// <returns></returns>
		const bool CheckDeviceExtensionSupport(VulkanDevice _Device, const std::vector<const char*> _DeviceExtensions);

		/// <summary>
		/// Selects the best format available for the swapchain
		/// </summary>
		/// <param name="_AvailableFormats">: All the availables formats of the swap chain</param>
		/// <returns></returns>
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _AvailableFormats);

		/// <summary>
		/// Selects the best presentation mode for the swap chain
		/// </summary>
		/// <param name="_AvailablePresentModes">: All the presentation modes of the swap chain</param>
		/// <returns></returns>
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _AvailablePresentModes);

		/// <summary>
		/// Selects the image format for the swap chain
		/// </summary>
		/// <param name="capabilities">: Swap chain capabilities availables </param>
		/// <returns></returns>
		VkExtent2D ChooseSwapExtent(Window* _Window, const VkSurfaceCapabilitiesKHR& _Capabilities);

		/// <summary>
		/// Creates all the swap chain frames buffers for each image view
		/// </summary>
		void CreateSwapChainFramebuffers();

		/// <summary>
		/// Creates the image views of the swap chain
		/// </summary>
		void CreateSwapChainImageViews(VkDevice& _Device);

	public:

		~VulkanSwapChain() override;

		inline VkFormat GetSwapChainFormat() { return m_SwapChainImageFormat; }

		/// <summary>
		/// Creates the Swap chain for our program
		/// </summary>
		/// <param name="_Window">: Current window context </param>
		/// <param name="_Device">: Application physical device </param>
		/// <param name="_Surface">: Rendering surface </param>
		const RHI_RESULT CreateSwapChain(Window* _Window, IDevice* _Device) override;

		/// <summary>
		/// Recreates the Swap chain
		/// </summary>
		/// <param name="_Window">: Current window context </param>
		const RHI_RESULT RecreateSwapChain(Window* _Window, IDevice* _Device, IPipeline* _Pipeline) override;

		/// <summary>
		/// Destroys all data linked to the swap chain and the swap chain
		/// </summary>
		const RHI_RESULT DestroySwapChain(IDevice* _Device) override;

		inline VulkanSwapChain* CastToVulkan() override { return this; }

		inline size_t GetSwapchainImagesNbr() { return m_SwapChainImages.size(); }

		/// <summary>
		/// Checks if the SwapChain is supported bu our GPU
		/// </summary>
		/// <param name="_Device">: Logical device which we check if the swap chain is supported </param>
		/// <param name="_Surface">: Rendering surface </param>
		/// <returns></returns>
		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _Device, VkSurfaceKHR _Surface);
	};
}