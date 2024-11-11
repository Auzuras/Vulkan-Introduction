#pragma once

#include "IResource.h"
#include "RHI/RHITypes.h"

namespace Core
{
	class VulkanShader;

	class IShader : public IResource
	{
	private:
	public:
		/// <summary>
		/// Loads a GLSL shader specified with a path
		/// </summary>
		/// <param name="_ResourcePath">: Path of the 3D model </param>
		/// <returns></returns>
		virtual const bool Load(Core::IDevice* _Device, std::filesystem::path _ResourcePath) override;

		/// <summary>
		/// Unloads the 3D model
		/// </summary>
		/// <returns></returns>
		const bool Unload() override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Device"></param>
		/// <param name="_ShaderName"></param>
		/// <param name="_ShaderSourceCode"></param>
		/// <param name="_ShaderType"></param>
		/// <returns></returns>
		virtual const bool CompileShader(Core::IDevice* _Device, const char* _ShaderName, std::string _ShaderSourceCode, Core::ShaderType _ShaderType) = 0;

		virtual VulkanShader* CastToVulkan() = 0;
	};
}