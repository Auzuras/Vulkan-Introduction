#pragma once

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDevice
	{
	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice;

		void PickPhysicalDevice(VkInstance _VulkanInstance);
		//const bool IsDeviceSuitable(VkPhysicalDevice _Device);

	public:
		inline VkPhysicalDevice& GetPhysicalDevice() { return m_PhysicalDevice; }
		inline VkDevice& GetLogicalDevice() { return m_LogicalDevice; }

		void Initialize(VkInstance _VulkanInstance);
		void Terninate();
	};
}