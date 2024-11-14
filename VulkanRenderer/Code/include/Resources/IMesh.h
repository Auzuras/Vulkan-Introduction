#pragma once

#include "IResource.h"

#include "RHI/Vertex.h"

namespace Core 
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
		const bool Unload(Core::IDevice* _Device) override;

		virtual RHI_RESULT CreateVertexBuffer(Core::IDevice* _Device, std::vector<Vertex>& _VerticesList) = 0;
		virtual RHI_RESULT CreateIndexBuffer(Core::IDevice* _Device, std::vector<uint32_t>& _IndicesList) = 0;
		
		virtual RHI_RESULT DestroyBuffers(Core::IDevice* _Device) = 0;
	};
}