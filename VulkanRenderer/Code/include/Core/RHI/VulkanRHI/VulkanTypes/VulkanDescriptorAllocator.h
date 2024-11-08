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
		/// <summary>
		/// Creates all descriptor pool
		/// </summary>
		const RHI_RESULT CreateDescriptorAllocator() override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const RHI_RESULT  DestroyDescriptorAllocator() override;
	};
}