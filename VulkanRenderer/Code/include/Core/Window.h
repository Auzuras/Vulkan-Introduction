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
		uint32_t m_Width = 800;
		uint32_t m_Height = 600;
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

		Window(const uint32_t _Width, const uint32_t _Height, const char* _WindowName);

		inline GLFWwindow* GetWindowPointer() { return m_Window; }

		/// <summary>
		/// Returns window width
		/// </summary>
		/// <returns></returns>
		const int GetWindowWidth() const;

		/// <summary>
		/// Sets window width
		/// </summary>
		/// <param name="_Width"></param>
		void SetWindowWidth(const uint32_t _Width);

		/// <summary>
		/// Returns window height
		/// </summary>
		/// <returns></returns>
		const int GetWindowHeight() const;

		/// <summary>
		/// Sets window height
		/// </summary>
		/// <param name="_Height"></param>
		void SetWindowHeight(const uint32_t _Height);

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
		const bool Initialize(RendererType _Type);

		/// <summary>
		/// Terminates the window
		/// </summary>
		/// <returns></returns>
		const bool Terminate();
	};
}