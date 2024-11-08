#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDescriptorAllocator
	{
	public:
		virtual const RHI_RESULT CreateDescriptorAllocator() = 0;
		virtual const RHI_RESULT DestroyDescriptorAllocator() = 0;
	};
}