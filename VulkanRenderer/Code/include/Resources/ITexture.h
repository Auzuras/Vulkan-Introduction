#pragma once

#include "IResource.h"

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDescriptor;
	class VulkanTexture;

	class ITexture : public IResource
	{
	private:

	protected:
		std::vector<IDescriptor*> p_Descriptors;

	public:
		/// <summary>
		/// Loads a texture with STB Image specified with a path
		/// </summary>
		/// <param name="_ResourcePath">: Path of the 3D model </param>
		/// <returns></returns>
		virtual const bool Load(IDevice* _Device, std::filesystem::path _ResourcePath);

		/// <summary>
		/// Unloads the 3D model
		/// </summary>
		/// <returns></returns>
		virtual const bool Unload(IDevice* _Device);

		virtual RHI_RESULT CreateTexture(IDevice* _Device, unsigned char* _TextureData, int _Width, int _Height) = 0;

		virtual RHI_RESULT DestroyTexture(IDevice* _Device) = 0;

		inline IDescriptor* GetDescriptor(unsigned int _CurrentFrame) { return p_Descriptors[_CurrentFrame]; }

		void DestroyDescriptor();

		virtual VulkanTexture* CastToVulkan() = 0;
	};
}