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
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const RHI_RESULT CreateCommandAllocator() override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const RHI_RESULT  DestroyCommandAllocator() override;
	};
}