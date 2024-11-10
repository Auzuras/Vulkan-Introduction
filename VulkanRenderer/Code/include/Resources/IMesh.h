#pragma once

#include "IResource.h"

namespace Resources
{
	class IMesh : public IResource
	{
	private:
	public:
		/// <summary>
		/// Loads a 3D model with TINY OBJ specified with a path
		/// </summary>
		/// <param name="_ResourcePath">: Path of the 3D model </param>
		/// <returns></returns>
		const bool Load(Core::IDevice* _Device, std::filesystem::path _ResourcePath) override;
		
		/// <summary>
		/// Unloads the 3D model
		/// </summary>
		/// <returns></returns>
		const bool Unload() override;
	};
}