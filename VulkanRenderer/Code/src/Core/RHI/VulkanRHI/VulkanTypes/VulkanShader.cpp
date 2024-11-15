#include "RHI/VulkanRHI/VulkanTypes/VulkanShader.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

namespace Core
{
	RHI_RESULT VulkanShader::PreprocessShader(shaderc::Compiler& _Compiler, CompilationInfos _Infos)
	{
		// First step - Preprocessing GLSL
		shaderc::PreprocessedSourceCompilationResult result = _Compiler.PreprocessGlsl(*_Infos.sourceCode, _Infos.shaderKind, _Infos.fileName, _Infos.options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to preprocessed shader, %s", result.GetErrorMessage());
			return RHI_FAILED_UNKNOWN;
		}

		// Copy the precompiled code in the buffer
		const char* src = result.cbegin();
		size_t newSize = result.cend() - src;
		_Infos.sourceCode->resize(newSize);
		memcpy(_Infos.sourceCode->data(), src, newSize);

		return RHI_SUCCESS;
	}

	RHI_RESULT VulkanShader::SpirVAssemblyCompilation(shaderc::Compiler& _Compiler, CompilationInfos _Infos)
	{
		// Second step - SPIR-V Assembly compilation
		shaderc::AssemblyCompilationResult sResult = _Compiler.CompileGlslToSpvAssembly(*_Infos.sourceCode, _Infos.shaderKind, _Infos.fileName, _Infos.options);

		if (sResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to compiled in assembly shader, %s", sResult.GetErrorMessage());
			return RHI_FAILED_UNKNOWN;
		}

		// Copy the SPRIV Assembly code in the buffer
		const char* src = sResult.cbegin();
		size_t newSize = sResult.cend() - src;
		_Infos.sourceCode->resize(newSize);
		memcpy(_Infos.sourceCode->data(), src, newSize);

		return RHI_SUCCESS;
	}

	std::vector<uint32_t> VulkanShader::SpirVBinaryCompilation(shaderc::Compiler& _Compiler, CompilationInfos _Infos)
	{
		// Third step - SPIR-V Binary compilation
		shaderc::SpvCompilationResult tResult = _Compiler.AssembleToSpv(_Infos.sourceCode->data(), _Infos.sourceCode->size(), _Infos.options);

		if (tResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to compiled in binary shader, %s", tResult.GetErrorMessage());
		}

		// Copy the SPIRV Binary code in the output buffer
		const uint32_t* srcBinary = tResult.cbegin();
		size_t wordCount = tResult.cend() - srcBinary;
		std::vector<uint32_t> output(wordCount);
		memcpy(output.data(), srcBinary, wordCount * sizeof(uint32_t));

		return output;
	}

	const bool VulkanShader::CompileShader(Core::IDevice* _Device, const char* _ShaderName, std::string _ShaderSourceCode, Core::ShaderType _ShaderType)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Causes memory leaks, thx
		shaderc::Compiler compiler;

		// Fill a struct of informations about the shader
		CompilationInfos infos{};
		infos.fileName = _ShaderName;
		infos.sourceCode = &_ShaderSourceCode;
		// TODO: Checks the optimizations
		//infos.options.SetOptimizationLevel(shaderc_optimization_level_performance);
		//infos.options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

		switch (_ShaderType)
		{
		case RHI_VERTEX: default:
			infos.shaderKind = shaderc_vertex_shader;
			break;
		case RHI_FRAGMENT:
			infos.shaderKind = shaderc_fragment_shader;
			break;
		case RHI_GEOMETRY:
			infos.shaderKind = shaderc_geometry_shader;
			break;
		}

		if (!PreprocessShader(compiler, infos))
			return false;

		if (!SpirVAssemblyCompilation(compiler, infos))
			return false;

		std::vector<uint32_t> compiledShader = SpirVBinaryCompilation(compiler, infos);

		if (compiledShader.empty())
			return false;

		CreateShaderModule(device, compiledShader);

		return true;
	}

	RHI_RESULT VulkanShader::CreateShaderModule(VulkanDevice _Device, const std::vector<uint32_t>& _ShaderBinaryCode)
	{
		// Create info of the shader
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		// Specifies the size and the code of the source code
		createInfo.codeSize = _ShaderBinaryCode.size() * sizeof(uint32_t);
		createInfo.pCode = _ShaderBinaryCode.data();

		VkResult result = vkCreateShaderModule(_Device.GetLogicalDevice(), &createInfo, nullptr, &m_ShaderModule);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create shader module, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	RHI_RESULT VulkanShader::DestroyShaderModule(Core::IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroyShaderModule(device.GetLogicalDevice(), m_ShaderModule, nullptr);

		return RHI_SUCCESS;
	}
}