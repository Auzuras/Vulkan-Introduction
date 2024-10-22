#pragma once
#include <vulkan/vulkan.h>

#include "RHI/RHI.h"

#include <vector>
#include <optional>

namespace Core
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		bool isComplete = false;
	};

	class VulkanWrapper : public RHI
	{
	private:
		VkInstance m_VulkanInstance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice;
		VkQueue m_GraphicsQueue;

		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif


	public:
		VulkanWrapper() = default;
		~VulkanWrapper() = default;

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		const bool Initialize() override;

		/// <summary>
		/// Creates a vulkan instance
		/// </summary>
		const bool CreateVulkanInstance();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const bool SetupDebugMessenger();

		const bool Terminate() override;

		///////////////////////////////////////////////////////////////////////

		/// Validation layers related methods
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const bool CheckValidationLayerSupport();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_MessageSeverity"></param>
		/// <param name="_MessageType"></param>
		/// <param name="_CallbackData"></param>
		/// <param name="_UserData"></param>
		/// <returns></returns>
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT _MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT _MessageType, const VkDebugUtilsMessengerCallbackDataEXT* _CallbackData, void* _UserData);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Instance"></param>
		/// <param name="_CreateInfo"></param>
		/// <param name="_Allocator"></param>
		/// <param name="_DebugMessenger"></param>
		/// <returns></returns>
		VkResult CreateDebugUtilsMessengerEXT(VkInstance _Instance, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo, const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Instance"></param>
		/// <param name="_DebugMessenger"></param>
		/// <param name="_Allocator"></param>
		void DestroyDebugUtilsMessengerEXT(VkInstance _Instance, VkDebugUtilsMessengerEXT _DebugMessenger, const VkAllocationCallbacks* _Allocator);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_CreateInfo"></param>
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _CreateInfo);

		///////////////////////////////////////////////////////////////////////

		/// Extensions related methods
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::vector<const char*> GetRequiredExtensions();

		///////////////////////////////////////////////////////////////////////

		/// Physical divice related methods
		
		///////////////////////////////////////////////////////////////////////

		void PickPhysicalDevice();
		const bool IsDeviceSuitable(VkPhysicalDevice _Device);

		///////////////////////////////////////////////////////////////////////

		/// Families queues related methods
		
		///////////////////////////////////////////////////////////////////////

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _Device);

		///////////////////////////////////////////////////////////////////////

		/// Logical devices related methods
		
		///////////////////////////////////////////////////////////////////////

		void CreateLogicalDevice();
	};
}