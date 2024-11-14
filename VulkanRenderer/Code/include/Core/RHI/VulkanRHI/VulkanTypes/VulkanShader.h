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

	class VulkanShader : public IShader
	{
	private:
		VkShaderModule m_ShaderModule;

	public:
		inline VkShaderModule GetShaderModule() { return m_ShaderModule; }

		RHI_RESULT PreprocessShader(shaderc::Compiler& _Compiler, CompilationInfos _Infos);
		RHI_RESULT SpirVAssemblyCompilation(shaderc::Compiler& _Compiler, CompilationInfos _Infos);
		std::vector<uint32_t> SpirVBinaryCompilation(shaderc::Compiler& _Compiler, CompilationInfos _Infos);

		const bool CompileShader(Core::IDevice* _Device, const char* _ShaderName, std::string _ShaderSourceCode, Core::ShaderType _ShaderType) override;

		RHI_RESULT CreateShaderModule(VulkanDevice _Device, const std::vector<uint32_t>& _ShaderBinaryCode);
		RHI_RESULT DestroyShaderModule(Core::IDevice* _Device) override;

		inline VulkanShader* CastToVulkan() override { return this; }
	};
}