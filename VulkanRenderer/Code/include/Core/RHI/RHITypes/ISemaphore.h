#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;
	class VulkanSemaphore;

	class ISemaphore
	{
	public:
		virtual ~ISemaphore() = default;

		virtual RHI_RESULT CreateSemaphoreSync(IDevice* _Device) = 0;
		virtual RHI_RESULT DestroySemaphoreSync(IDevice* _Device) = 0;
		virtual VulkanSemaphore* CastToVulkan() = 0;

	};
}