#pragma once

#include "RHI/RHITypes/RHIResult.h"
#include "RHI/RHITypes.h"

#include <vector>

namespace Core
{
	class IDevice;
	class ISwapChain;
	class IShader;
	class VulkanPipeline;
	class IDescriptorLayout;

	struct PipelineShaderInfos
	{
		IShader* shader;
		ShaderType shaderType;
		const char* functionEntry;
	};

	class IPipeline
	{
	protected:
		std::vector<IDescriptorLayout*> p_DescriptorSetLayouts{};

	public:
		virtual ~IPipeline() = default;

		virtual RHI_RESULT CreatePipeline(IDevice* _Device, ISwapChain* _Swapchain, std::vector<PipelineShaderInfos> _ShadersInfos) = 0;
		virtual RHI_RESULT DestroyPipeline(IDevice* _Device) = 0;

		virtual VulkanPipeline* CastToVulkan() = 0;

		virtual std::vector<IDescriptorLayout*> GetDescriptorLayouts() { return p_DescriptorSetLayouts; }
	};
}