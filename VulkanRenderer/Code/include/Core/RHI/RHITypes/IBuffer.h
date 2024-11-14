#pragma once

#include "RHI/RHITypes/RHIResult.h"
#include "RHI/RHITypes.h"

namespace Core
{
	class IDevice;

	class IBuffer
	{
	private:
	public:
		virtual const RHI_RESULT CreateBuffer(IDevice* _Device, BufferType _BufferType, size_t _BufferSize) = 0;
		virtual const RHI_RESULT DestroyBuffer(IDevice* _Device) = 0;
	};
}