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

		/// <summary>
		/// Creates a single command buffer
		/// </summary>
		ICommandBuffer* CreateCommandBuffer(IDevice* _Device) override;

		/// <summary>
		/// Creates a list of command buffers
		/// </summary>
		std::vector<ICommandBuffer*> CreateCommandBuffers(IDevice* _Device, int _CommandBuffersNbr) override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		inline VulkanCommandAllocator* CastToVulkan() override { return this; };

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		inline VkCommandPool& GetCommandAllocator() { return m_CommandPool; }
	};
}