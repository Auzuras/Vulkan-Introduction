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
		Model(Core::IMesh* _Mesh, Core::ITexture* _Texture);

		void Update() override;
		void Draw() override;
	};
}