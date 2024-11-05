#include "Window.h"

namespace Core
{
	Window::Window(const uint32_t _Width, const uint32_t _Height, const char* _WindowName)
		:m_Width(_Width), m_Height(_Height), m_WindowName(_WindowName)
	{}

	const bool Window::InitializeVulkanWindow()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		return true;
	}

	const bool Window::InitializeOpenGLWindow()
	{
		// TODO: Initialize OpenGL window
		return false;
	}

	const bool Window::InitializeDirectXWindow()
	{
		// TODO: Initialize DirectX window
		return false;
	}

	const char** Window::GetWindowExtensions()
	{
		const char** glfwExtensions;
		uint32_t glfwExtensionCount = 0;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		return glfwExtensions;
	}

	const bool Window::Initialize(RendererType _Type)
	{
		glfwInit();

		switch (_Type)
		{
		case VULKAN: default:
			InitializeVulkanWindow();
			break;
		case OPENGL:
			InitializeOpenGLWindow();
			break;
		case DIRECTX:
			InitializeDirectXWindow();
			break;
		}

		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName, nullptr, nullptr);

		if (!m_Window)
		{
			DEBUG_ERROR("Failed to create window !")
			return false;
		}

		return true;
	}

	const bool Window::Terminate()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();

		return true;
	}
}