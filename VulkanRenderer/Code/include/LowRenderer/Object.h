#pragma once

#include "Transform.h"
#include "Application.h"


namespace LowRenderer
{
	class Object
	{
	private:
	public:

#ifdef OPENGL_WRAPPER
		Physics::Transform transform;
#endif

#ifdef VULKAN_WRAPPER
		int a = 9;
		int b = 9;
		int c = 9;
#endif
	};
}