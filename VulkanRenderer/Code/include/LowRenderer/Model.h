#pragma once

#include "Object.h"
#include "IMesh.h"
#include "ITexture.h"

#include "Matrices/Matrix4.h"

namespace LowRenderer
{
	class Model : public Object
	{
	private:
		Core::IMesh* m_Mesh;
		Core::ITexture* m_Texture;

	public:
		Model() = default;
		Model(Core::IMesh* _Mesh, Core::ITexture* _Texture);

		inline Core::IMesh* GetMesh() { return m_Mesh; }
		inline Core::ITexture* GetTexture() { return m_Texture; }

		inline void SetMesh(Core::IMesh* _Mesh) { m_Mesh = _Mesh; }
		inline void SetTexture(Core::ITexture* _Texture) { m_Texture = _Texture; }

		void DestroyDescriptors();

		void Update() override;
		void Draw() override;
	};
}