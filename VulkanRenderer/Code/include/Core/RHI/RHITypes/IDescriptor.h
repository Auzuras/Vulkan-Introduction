#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;

	class IDescriptor
	{
	public:
		virtual ~IDescriptor() = default;

		virtual const RHI_RESULT CreateDescriptorLayout(IDevice* _Device) = 0;
	};
}