#pragma once

#include "Object.h"
#include "Matrices/Matrix4.h"

namespace LowRenderer
{
	class Camera : public Object
	{
	private:
		Math::Matrix4 m_ViewMatrix = Math::Matrix4::identity;
		Math::Matrix4 m_ProjectionMatrix;
	public:
	};
}