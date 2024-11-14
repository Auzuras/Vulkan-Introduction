#pragma once

#include "RHI/RHITypes/IDescriptorAllocator.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDescriptorAllocator : public IDescriptorAllocator
	{
	private:
		VkDescriptorPool m_DescriptorPool;

	public:

		~VulkanDescriptorAllocator() override;

		/// <summary>
		/// Creates all descriptor pool
		/// </summary>
		const RHI_RESULT CreateDescriptorAllocator(IDevice* _Device, ISwapChain* _Swapchain) override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const RHI_RESULT DestroyDescriptorAllocator(IDevice* _Device) override;
	};
}