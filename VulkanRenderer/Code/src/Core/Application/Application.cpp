#include "Application/Application.h"

#include "RHI/VulkanWrapper.h"

namespace Core
{
	Application::Application(int _Width, int _Height, const char* _WindowName, RHIType _RHIType)
		:m_Width(_Width), m_Height(_Height), m_WindowName(_WindowName), m_RHIType(_RHIType)
	{}

	Application::Application(int _Width, int _Height)
		:m_Width(_Width), m_Height(_Height)
	{}

	const bool Application::Initialize()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName, nullptr, nullptr);

		switch (m_RHIType)
		{
		case Core::VULKAN: default:
			m_RHI = new Core::VulkanWrapper;
			break;
		}

		if (!m_RHI)
		{
			// TODO: ERR
			return false;
		}

		if (!m_RHI->Initialize())
		{
			// TODO: ERR
			return false;
		}

		// TODO: Initialization

		return true;
	}

	const bool Application::Terminate()
	{
		bool returnValue = true;

		if (!m_RHI->Terminate())
		{
			// TODO: ERR
			returnValue = false;
		}

		delete m_RHI;

		glfwDestroyWindow(m_Window);
		glfwTerminate();

		return returnValue;
	}
}