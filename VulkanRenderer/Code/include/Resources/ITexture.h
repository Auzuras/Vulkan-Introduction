#pragma once

#include "IResource.h"

namespace Core
{
	class ITexture : public IResource
	{
	private:
	public:
		/// <summary>
		/// Loads a texture with STB Image specified with a path
		/// </summary>
		/// <param name="_ResourcePath">: Path of the 3D model </param>
		/// <returns></returns>
		virtual const bool Load(Core::IDevice* _Device, std::filesystem::path _ResourcePath) override = 0;

		/// <summary>
		/// Unloads the 3D model
		/// </summary>
		/// <returns></returns>
		virtual const bool Unload() = 0;
	};
}