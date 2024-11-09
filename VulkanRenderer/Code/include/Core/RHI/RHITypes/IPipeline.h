#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;
	class IShader;
	class VulkanPipeline;

	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;

		virtual RHI_RESULT CreatePipeline(IDevice* _Device) = 0;
		virtual RHI_RESULT DestroyPipeline(IDevice* _Device) = 0;

		virtual VulkanPipeline* CastToVulkan() = 0;
	};
}