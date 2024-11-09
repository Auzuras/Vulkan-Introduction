#pragma once

#include "RHI/VulkanRHI/VulkanRenderer.h"
#include "RHI/RHITypes/IDevice.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanQueue.h"

namespace Core
{
	class VulkanDevice : public IDevice
	{
	private:

		VkInstance m_VulkanInstance;

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice;

		VkSurfaceKHR m_Surface;

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif

		VkDebugUtilsMessengerEXT m_DebugMessenger;

		const std::vector<const char*> m_DeviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		const std::vector<const char*> m_ValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
		};

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentationQueue;
		VkQueue m_TransferQueue;

		///////////////////////////////////////////////////////////////////////

		/// Setup related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a vulkan instance that links the application with the API
		/// </summary>
		const RHI_RESULT CreateVulkanInstance();

		/// <summary>
		/// Setup debugger tool for validation layers
		/// </summary>
		/// <returns></returns>
		const RHI_RESULT SetupDebugMessenger();

		///////////////////////////////////////////////////////////////////////

		/// Validation layers / Debug related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Checks if validation layers are supported with the SDK
		/// </summary>
		/// <returns></returns>
		const bool CheckValidationLayerSupport();

		/// <summary>
		/// Specifies all the information for the creation of the debug messenger
		/// </summary>
		/// <param name="_CreateInfo">: Create info structure of the debug messenger </param>
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _CreateInfo);

		/// <summary>
		/// Callback method called when a validation layer should be print
		/// </summary>
		/// <param name="_MessageSeverity">: Type of severity of the message, LOG, WARN, EROOR </param>
		/// <param name="_MessageType">: Type of the message </param>
		/// <param name="_CallbackData">: Data of the message </param>
		/// <param name="_UserData">: More data from the user </param>
		/// <returns></returns>
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT _MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT _MessageType, const VkDebugUtilsMessengerCallbackDataEXT* _CallbackData, void* _UserData);

		/// <summary>
		/// Creates the debug messenger for validation layers
		/// </summary>
		/// <param name="_Instance">: Vulkan instance </param>
		/// <param name="_CreateInfo">: Creation information structure - Specifies all the informations for the debug messenger </param>
		/// <param name="_Allocator">: Pointer to a structure for custom memory allocations </param>
		/// <param name="_DebugMessenger">: Debug messenger you want to create </param>
		/// <returns></returns>
		VkResult CreateDebugUtilsMessengerEXT(VkInstance _Instance, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo, const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger);

		/// <summary>
		/// Destroys the debug messenger and the callback method
		/// </summary>
		/// <param name="_Instance">: Vulkan instance of the program </param>
		/// <param name="_DebugMessenger">: Debug messenger you want to destroy </param>
		/// <param name="_Allocator">: Pointer to a structure for custom memory allocations </param>
		void DestroyDebugUtilsMessengerEXT(VkInstance _Instance, VkDebugUtilsMessengerEXT _DebugMessenger, const VkAllocationCallbacks* _Allocator);

		///////////////////////////////////////////////////////////////////////

		/// 

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Selects a GPU of the computer in order to use it
		/// </summary>
		const RHI_RESULT PickPhysicalDevice();

		/// <summary>
		/// Checks if a GPU is able to handle the operations required for our application
		/// </summary>
		/// <param name="_Device">: Physical device that will be checked </param>
		/// <returns></returns>
		const bool IsDeviceSuitable(VkPhysicalDevice _Device);

		/// <summary>
		/// Creates a logical device used to interface with Vulkan and our RHI
		/// </summary>
		const RHI_RESULT CreateLogicalDevice();

		/// <summary>
		/// Creates a surface which is going to link Vulkan with Window API
		/// </summary>
		/// <param name="_Window">: Current window context </param>
		void CreateSurface(Window* _Window);

		///////////////////////////////////////////////////////////////////////

		/// Extensions related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Get all required extensions needed for the application (GLFW and Debug extensions)
		/// </summary>
		/// <returns></returns>
		std::vector<const char*> GetRequiredExtensions();

		/// <summary>
		/// Checks if the extensions needed for our program are supported by our GPU
		/// </summary>
		/// <param name="_Device">: Logical device which we check extensions support </param>
		/// <returns></returns>
		const bool CheckDeviceExtensionSupport(VkPhysicalDevice _Device);

	public:

		///////////////////////////////////////////////////////////////////////

		/// Constructors and destructors

		///////////////////////////////////////////////////////////////////////

		~VulkanDevice() override;

		///////////////////////////////////////////////////////////////////////

		/// Getters and setters

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Returns vulkan physical device
		/// </summary>
		/// <returns></returns>
		inline VkPhysicalDevice& GetPhysicalDevice() { return m_PhysicalDevice; }

		/// <summary>
		/// Returns vulkan logical device
		/// </summary>
		/// <returns></returns>
		inline VkDevice& GetLogicalDevice() { return m_LogicalDevice; }

		inline VkSurfaceKHR& GetSurface() { return m_Surface; }

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Initializes vulkan device
		/// </summary>
		const RHI_RESULT Initialize(Window* _Window) override;

		/// <summary>
		/// Terminates vulkan device
		/// </summary>
		const RHI_RESULT Terminate() override;

		inline VulkanDevice* CastToVulkan() override { return this; }
	};
}