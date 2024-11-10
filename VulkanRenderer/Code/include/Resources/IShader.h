#pragma once

#include "IResource.h"
#include "RHI/RHITypes.h"

namespace Resources
{
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
	};
}