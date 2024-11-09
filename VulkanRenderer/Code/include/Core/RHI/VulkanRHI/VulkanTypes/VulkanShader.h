#pragma once

#include "IShader.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

#include <shaderc/shaderc.hpp>

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
		RHI_RESULT PreprocessShader(shaderc::Compiler& _Compiler, CompilationInfos _Infos);
		RHI_RESULT SpirVAssemblyCompilation(shaderc::Compiler& _Compiler, CompilationInfos _Infos);
		std::vector<uint32_t> SpirVBinaryCompilation(shaderc::Compiler& _Compiler, CompilationInfos _Infos);
	};
}