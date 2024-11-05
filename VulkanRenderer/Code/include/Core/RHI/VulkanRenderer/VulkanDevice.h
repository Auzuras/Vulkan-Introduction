#pragma once

#include "RHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDevice
	{
	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice;

	public:
	};
}