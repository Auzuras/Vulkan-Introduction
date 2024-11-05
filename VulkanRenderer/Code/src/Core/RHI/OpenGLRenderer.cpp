#include "RHI/OpenGLRenderer.h"

Core::OpenGLRenderer::~OpenGLRenderer()
{
}

const bool Core::OpenGLRenderer::Initialize(Window* _Window)
{
	return true;
}

const bool Core::OpenGLRenderer::Terminate()
{
	return false;
}

void Core::OpenGLRenderer::DrawFrame(Window* _Window)
{
}
