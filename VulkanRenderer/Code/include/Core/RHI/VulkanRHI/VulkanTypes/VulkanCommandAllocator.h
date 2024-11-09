#pragma once

#include "RHI/RHITypes/ICommandAllocator.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanCommandAllocator : public ICommandAllocator
	{
	private:
		VkCommandPool m_CommandPool;

	public:

		~VulkanCommandAllocator() override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const RHI_RESULT CreateCommandAllocator(IDevice* _Device) override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const RHI_RESULT  DestroyCommandAllocator(IDevice* _Device) override;

		inline VulkanCommandAllocator* CastToVulkan() override { return this; };

		inline VkCommandPool GetCommandPool() { return m_CommandPool; }
	};
}