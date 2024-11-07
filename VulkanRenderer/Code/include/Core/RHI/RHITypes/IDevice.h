#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice
	{
	public:
		virtual ~IDevice() = default;

		virtual const RHI_RESULT Initialize(Window* _Window) = 0;
		virtual const RHI_RESULT Terminate() = 0;

		virtual VulkanDevice* CastToVulkan() = 0;
	};
}