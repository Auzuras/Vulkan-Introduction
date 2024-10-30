#include "RHI/OpenGLWrapper.h"

Core::OpenGLWRapper::~OpenGLWRapper()
{
}

const bool Core::OpenGLWRapper::Initialize(GLFWwindow* _Window)
{
	return true;
}

const bool Core::OpenGLWRapper::Terminate()
{
	return false;
}

void Core::OpenGLWRapper::DrawFrame(GLFWwindow* _Window)
{
}
