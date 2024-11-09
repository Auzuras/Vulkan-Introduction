#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;

	class VulkanCommandAllocator;

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

		virtual VulkanCommandAllocator* CastToVulkan() = 0;
	};
}