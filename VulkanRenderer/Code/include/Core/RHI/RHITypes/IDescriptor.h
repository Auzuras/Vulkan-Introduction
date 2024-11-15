#pragma once

#include "RHI/RHITypes/RHIResult.h"
#include "RHI/RHITypes.h"

namespace Core
{
	class IDevice;
	class VulkanDescriptor;

	class IDescriptor
	{
	public:
		virtual ~IDescriptor() = default;

		virtual VulkanDescriptor* CastToVulkan() = 0;
	};
}