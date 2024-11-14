#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;
	class ISwapChain;

	class IDescriptorAllocator
	{
	public:
		virtual ~IDescriptorAllocator() = default;

		virtual const RHI_RESULT CreateDescriptorAllocator(IDevice* _Device, ISwapChain* _Swapchain) = 0;
		virtual const RHI_RESULT DestroyDescriptorAllocator(IDevice* _Device) = 0;
	};
}