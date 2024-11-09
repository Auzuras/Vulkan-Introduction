#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;
	class ICommandAllocator;

	class ICommandBuffer
	{
	public:

		virtual RHI_RESULT CreateCommandBuffers(IDevice* _Device, ICommandAllocator* _CommandAllocator) = 0;
	};
}