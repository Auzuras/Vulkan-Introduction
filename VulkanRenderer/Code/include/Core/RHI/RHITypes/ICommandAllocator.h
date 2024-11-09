#pragma once

#include "RHI/RHITypes/RHIResult.h"

#include <vector>

namespace Core
{
	class IDevice;

	class VulkanCommandAllocator;

	class ICommandBuffer;

	class ICommandAllocator
	{
	public:		
		
		virtual ~ICommandAllocator() = default;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const RHI_RESULT CreateCommandAllocator(IDevice* _Device) = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const RHI_RESULT  DestroyCommandAllocator(IDevice* _Device) = 0;

		/// <summary>
		/// Creates a single command buffer
		/// </summary>
		virtual ICommandBuffer* CreateCommandBuffer(IDevice* _Device) = 0;

		/// <summary>
		/// Creates a list of command buffers
		/// </summary>
		virtual std::vector<ICommandBuffer*> CreateCommandBuffers(IDevice* _Device, int _CommandBuffersNbr) = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual VulkanCommandAllocator* CastToVulkan() = 0;
	};
}