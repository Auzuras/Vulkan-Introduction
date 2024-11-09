#pragma once

#include "Debug/Log.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "RHI/RHITypes.h"

namespace Core
{
	class Window
	{
	private:
		int m_Width = 1280;
		int m_Height = 720;
		const char* m_WindowName = "DefaultName";

		GLFWwindow* m_Window = nullptr;

		/// <summary>
		/// Initialize GLFW window with vulkan specificities
		/// </summary>
		/// <returns></returns>
		const bool InitializeVulkanWindow();

		/// <summary>
		/// Initialize GLFW window with OpenGL specificities
		/// </summary>
		/// <returns></returns>
		const bool InitializeOpenGLWindow();

		/// <summary>
		/// Initialize GLFW window with DirectX specificities
		/// </summary>
		/// <returns></returns>
		const bool InitializeDirectXWindow();
	public:

		Window() = default;

		Window(const int _Width, const int _Height, const char* _WindowName);

		///////////////////////////////////////////////////////////////////////

		/// Getters and setters

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Returns the pointer to the GLFW window
		/// </summary>
		/// <returns></returns>
		inline GLFWwindow* GetWindowPointer() { return m_Window; }

		/// <summary>
		/// Returns window width
		/// </summary>
		/// <returns></returns>
		inline const int GetWindowWidth() const { return m_Width; }

		/// <summary>
		/// Sets window width
		/// </summary>
		/// <param name="_Width"></param>
		inline void SetWindowWidth(const int _Width) { m_Width = _Width; }

		/// <summary>
		/// Returns window height
		/// </summary>
		/// <returns></returns>
		inline const int GetWindowHeight() const { return m_Height; }

		/// <summary>
		/// Sets window height
		/// </summary>
		/// <param name="_Height"></param>
		inline void SetWindowHeight(const int _Height) { m_Height = _Height; }

		/// <summary>
		/// Returns window name
		/// </summary>
		/// <returns></returns>
		inline const char* GetWindowName() const { return m_WindowName; }

		/// <summary>
		/// Sets window name
		/// </summary>
		/// <param name="_WindowName"></param>
		inline void SetWindowName(const char* _WindowName) { m_WindowName = _WindowName; glfwSetWindowTitle(m_Window, _WindowName); }

		/// <summary>
		/// Returns the window extensions to initialize some APIs
		/// </summary>
		/// <returns></returns>
		const char** GetWindowExtensions();

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Initializes window according to the current RHI
		/// </summary>
		/// <param name="_Type"></param>
		/// <returns></returns>
		const bool Initialize(RendererType _Type);

		/// <summary>
		/// Terminates the window
		/// </summary>
		/// <returns></returns>
		const bool Terminate();
	};
}