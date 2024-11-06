#pragma once

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDevice
	{
	private:

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice;

		/// <summary>
		/// Selects a GPU of the computer in order to use it
		/// </summary>
		void PickPhysicalDevice(VkInstance _VulkanInstance);

		/// <summary>
		/// Checks if a GPU is able to handle the operations required for our application
		/// </summary>
		/// <param name="_Device">: Physical device that will be checked </param>
		/// <returns></returns>
		const bool IsDeviceSuitable(VkPhysicalDevice _Device);

		/// <summary>
		/// Creates a logical device used to interface with Vulkan and our RHI
		/// </summary>
		void CreateLogicalDevice();

	public:

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		inline VkPhysicalDevice& GetPhysicalDevice() { return m_PhysicalDevice; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		inline VkDevice& GetLogicalDevice() { return m_LogicalDevice; }

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_VulkanInstance"></param>
		void Initialize(VkInstance _VulkanInstance);

		/// <summary>
		/// 
		/// </summary>
		void Terninate();
	};
}