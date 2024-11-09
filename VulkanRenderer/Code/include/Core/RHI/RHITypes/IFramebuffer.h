#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;
	class IPipeline;

	class IFrameBuffer
	{
	public:
		virtual const RHI_RESULT CreateFramebuffer(IDevice* _Device, IPipeline* _CompatiblePipeline, uint32_t _Width, uint32_t _Height) = 0;
		virtual const RHI_RESULT DestroyFramebuffer(IDevice* _Device) = 0;
	};
}