#pragma once

#include "IShader.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	struct CompilationInfos
	{
		const char* fileName;
		shaderc_shader_kind shaderKind;
		std::string* sourceCode;
		shaderc::CompileOptions options;
	};

	class VulkanShader : public Resources::IShader
	{
	private:
	public:
	};
}