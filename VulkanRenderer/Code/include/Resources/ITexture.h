#pragma once

#include "IResource.h"

#include "RHI/RHITypes/RHIResult.h"

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
		virtual const bool Load(Core::IDevice* _Device, std::filesystem::path _ResourcePath);

		/// <summary>
		/// Unloads the 3D model
		/// </summary>
		/// <returns></returns>
		virtual const bool Unload(Core::IDevice* _Device);

		virtual RHI_RESULT CreateTexture(IDevice* _Device, unsigned char* _TextureData, int _Width, int _Height) = 0;

		virtual RHI_RESULT DestroyTexture(IDevice* _Device) = 0;
	};
}