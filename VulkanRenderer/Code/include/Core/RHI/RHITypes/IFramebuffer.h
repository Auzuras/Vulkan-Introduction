#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IFrameBuffer
	{
	public:
		virtual const RHI_RESULT CreateFramebuffer() = 0;
		virtual const RHI_RESULT DestroyFramebuffer() = 0;
	};
}