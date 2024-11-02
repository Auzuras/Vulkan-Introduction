#include "Window.h"

namespace Core
{
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

	const int Window::GetWindowWidth() const
	{
		return m_Width;
	}

	void Window::SetWindowWidth(int _Width)
	{
		m_Width = _Width;
	}

	const int Window::GetWindowHeight() const
	{
		return m_Height;
	}

	void Window::SetWindowHeight(int _Height)
	{
		m_Height = _Height;
	}

	const const char* Window::GetWindowName() const
	{
		return m_WindowName;
	}

	void Window::SetWindowName(const char* _WindowName)
	{
		m_WindowName = _WindowName;
	}

	const char** Window::GetWindowExtensions()
	{
		const char** glfwExtensions;
		uint32_t glfwExtensionCount = 0;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		return glfwExtensions;
	}

	const bool Window::Initialize(RHIType _Type)
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