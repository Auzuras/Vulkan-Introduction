#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class Window;
	class IDevice;

	class ISwapChain
	{
	public:
		virtual const RHI_RESULT CreateSwapChain(Window* _Window, IDevice* _Device) = 0;
		virtual const RHI_RESULT RecreateSwapChain(Window* _Window, IDevice* _Device) = 0;
		virtual const RHI_RESULT DestroySwapChain(IDevice* _Device) = 0;
	};
}