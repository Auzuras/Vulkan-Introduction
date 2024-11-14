#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;
	class VulkanFence;

	class IFence
	{
	public:
		virtual ~IFence() = default;

		virtual RHI_RESULT CreateFenceSync(IDevice* _Device) = 0;
		virtual RHI_RESULT DestroyFenceSync(IDevice* _Device) = 0;

		virtual void WaitFence(IDevice* _Device, unsigned int _CancelDelay) = 0;
		virtual void ResetFence(IDevice* _Device) = 0;

		virtual VulkanFence* CastToVulkan() = 0;
	};
}