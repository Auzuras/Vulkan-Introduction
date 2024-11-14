#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;
	class IPipeline;

	class IFrameBuffer
	{
	public:
		virtual ~IFrameBuffer() = default;

		//virtual const RHI_RESULT CreateFramebuffer(IDevice* _Device, IPipeline* _CompatiblePipeline, int _Width, int _Height) = 0;
		//virtual const RHI_RESULT DestroyFramebuffer(IDevice* _Device) = 0;
	};
}