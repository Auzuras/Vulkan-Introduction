#pragma once

#include "RHI/RHITypes.h"
#include "RHIResult.h"
#include <vector>

namespace Core
{
	struct DescriptorLayoutInfos
	{
		unsigned int binding;
		DescriptorType type;
		ShaderType entryShader;
	};

	class IDevice;
	class VulkanDescriptorLayout;

	class IDescriptorLayout
	{
	public:
		virtual ~IDescriptorLayout() = default;

		virtual const RHI_RESULT CreateDescriptorSetLayout(IDevice* _Device, std::vector<DescriptorLayoutInfos> _Infos) = 0;
		virtual const void DestroyDescriptorSetLayout(IDevice* _Device) = 0;
		
		virtual VulkanDescriptorLayout* CastToVulkan() = 0;
	};
}