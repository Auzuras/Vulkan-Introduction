#pragma once

#include "Debug/Log.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "RHI/RHI.h"

namespace Core
{
	class Window
	{
	private:
		int m_Width = 800;
		int m_Height = 600;
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

		/// <summary>
		/// Returns window width
		/// </summary>
		/// <returns></returns>
		const int GetWindowWidth() const;

		/// <summary>
		/// Sets window width
		/// </summary>
		/// <param name="_Width"></param>
		void SetWindowWidth(int _Width);

		/// <summary>
		/// Returns window height
		/// </summary>
		/// <returns></returns>
		const int GetWindowHeight() const;

		/// <summary>
		/// Sets window height
		/// </summary>
		/// <param name="_Height"></param>
		void SetWindowHeight(int _Height);

		/// <summary>
		/// Returns window name
		/// </summary>
		/// <returns></returns>
		const char* GetWindowName() const;

		/// <summary>
		/// Sets window name
		/// </summary>
		/// <param name="_WindowName"></param>
		void SetWindowName(const char* _WindowName);

		/// <summary>
		/// Returns the window extensions to initialize some APIs
		/// </summary>
		/// <returns></returns>
		const char** GetWindowExtensions();

		/// <summary>
		/// Initializes window according to the current RHI
		/// </summary>
		/// <param name="_Type"></param>
		/// <returns></returns>
		const bool Initialize(RHIType _Type);

		/// <summary>
		/// Terminates the window
		/// </summary>
		/// <returns></returns>
		const bool Terminate();
	};
}