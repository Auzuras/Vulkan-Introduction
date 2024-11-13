#pragma once

#include "RHI/RHITypes/RHIResult.h"
#include "RHI/RHITypes.h"

namespace Core
{
	class IDevice;

	enum DescriptorType
	{
		UNIFORM_BUFFER,
		SAMPLER
	};

	struct DescriptorLayoutInfos
	{
		int binding;
		int count;
		DescriptorType type;
		ShaderType shaderStage;
	};

	class IDescriptor
	{
	public:
		virtual ~IDescriptor() = default;
	};
}