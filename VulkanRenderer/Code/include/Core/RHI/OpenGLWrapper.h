#pragma once

#include "RHI/RHI.h"

namespace Core
{
	class OpenGLWRapper : public RHI
	{
		~OpenGLWRapper() override;

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		const bool Initialize(GLFWwindow* _Window) override;

		const bool Terminate() override;

		void DrawFrame(GLFWwindow* _Window) override;
	};
}