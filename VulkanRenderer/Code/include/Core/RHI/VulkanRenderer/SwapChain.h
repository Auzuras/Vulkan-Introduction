//#pragma once
//
//#include "RHI/VulkanRenderer.h"
//
//struct SwapChainSupportDetails
//{
//	VkSurfaceCapabilitiesKHR capabilities;
//	std::vector<VkSurfaceFormatKHR> formats;
//	std::vector<VkPresentModeKHR> presentModes;
//};
//
//struct QueueFamilyIndices
//{
//	std::optional<uint32_t> graphicsFamily;
//	std::optional<uint32_t> presentFamily;
//
//	bool isComplete = false;
//};
//
//class SwapChain
//{
//private:
//
//	/// <summary>
//	/// Checks if the extensions needed for our program are supported by our GPU
//	/// </summary>
//	/// <param name="_Device">: Logical device which we check extensions support </param>
//	/// <returns></returns>
//	const bool CheckDeviceExtensionSupport(VkPhysicalDevice _Device);
//
//	/// <summary>
//	/// Checks if the SwapChain is supported bu our GPU
//	/// </summary>
//	/// <param name="_Device">: Logical device which we check if the swap chain is supported </param>
//	/// <param name="_Surface">: Rendering surface </param>
//	/// <returns></returns>
//	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _Device, VkSurfaceKHR _Surface);
//
//	/// <summary>
//	/// Selects the best format available for the swapchain
//	/// </summary>
//	/// <param name="_AvailableFormats">: All the availables formats of the swap chain</param>
//	/// <returns></returns>
//	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _AvailableFormats);
//
//	/// <summary>
//	/// Selects the best presentation mode for the swap chain
//	/// </summary>
//	/// <param name="_AvailablePresentModes">: All the presentation modes of the swap chain</param>
//	/// <returns></returns>
//	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _AvailablePresentModes);
//
//	/// <summary>
//	/// Selects the image format for the swap chain
//	/// </summary>
//	/// <param name="capabilities">: Swap chain capabilities availables </param>
//	/// <returns></returns>
//	VkExtent2D ChooseSwapExtent(GLFWwindow* _Window, const VkSurfaceCapabilitiesKHR& _Capabilities);
//
//public:
//
//	/// <summary>
//	/// Creates the Swap chain for our program
//	/// </summary>
//	/// <param name="_Window">: Current window context </param>
//	/// <param name="_Device">: Application physical device </param>
//	/// <param name="_Surface">: Rendering surface </param>
//	void CreateSwapChain(GLFWwindow* _Window, VkPhysicalDevice _Device, VkSurfaceKHR _Surface);
//
//	/// <summary>
//	/// Recreates the Swap chain
//	/// </summary>
//	/// <param name="_Window">: Current window context </param>
//	void RecreateSwapChain(GLFWwindow* _Window);
//
//	/// <summary>
//	/// Destroys all data linked to the swap chain and the swap chain
//	/// </summary>
//	void CleanSwapChain();
//};