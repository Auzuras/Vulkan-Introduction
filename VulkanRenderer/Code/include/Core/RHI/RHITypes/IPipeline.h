#pragma once

#include "RHI/RHITypes/RHIResult.h"
#include "RHI/RHITypes.h"

namespace Core
{
	class IDevice;
	class ISwapChain;
	class IShader;
	class VulkanPipeline;

	struct PipelineShaderInfos
	{
		IShader* shader;
		ShaderType shaderType;
		const char* functionEntry;
	};

	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;

		virtual RHI_RESULT CreatePipeline(IDevice* _Device, ISwapChain* _Swapchain) = 0;
		virtual RHI_RESULT DestroyPipeline(IDevice* _Device) = 0;

		virtual VulkanPipeline* CastToVulkan() = 0;
	};
}