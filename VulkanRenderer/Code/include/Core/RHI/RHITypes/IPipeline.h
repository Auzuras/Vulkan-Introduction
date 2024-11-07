#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IPipeline
	{
	public:
		virtual RHI_RESULT CreatePipeline() = 0;
		virtual RHI_RESULT DestroyPipeline() = 0;
	};
}