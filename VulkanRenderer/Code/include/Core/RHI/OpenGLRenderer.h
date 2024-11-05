#pragma once

#include "RHI/IRendererHardware.h"

namespace Core
{
	class OpenGLRenderer : public IRendererHardware
	{
		~OpenGLRenderer() override;

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		const bool Initialize(Window* _Window) override;

		const bool Terminate() override;

		void DrawFrame(Window* _Window) override;
	};
}