#pragma once

#include "RHI/RHITypes/IBuffer.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanBuffer : public IBuffer
	{
	private:

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;

	public:
	};
}