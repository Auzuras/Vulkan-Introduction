#pragma once

#include "Object.h"
#include "Matrices/Matrix4.h"

namespace LowRenderer
{
	class Camera : public Object
	{
	private:
	public:

		Math::Vector3 position = Math::Vector3(0.f, 0.f, -2.f);
		Camera();

		Math::Matrix4 viewMatrix = Math::Matrix4::identity;
		Math::Matrix4 projectionMatrix = Math::Matrix4::identity;

		void Update();
	};
}