#include "RHI/OpenGLRHI/OpenGLRenderer.h"

namespace Core
{
	OpenGLRenderer::~OpenGLRenderer()
	{
	}

	const bool OpenGLRenderer::Initialize(Window* _Window)
	{
		return true;
	}

	const bool OpenGLRenderer::Terminate()
	{
		return false;
	}

	void OpenGLRenderer::DrawFrame(Window* _Window)
	{
	}
}