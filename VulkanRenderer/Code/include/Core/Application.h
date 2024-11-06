#pragma once

#include "Window.h"

#include "RHI/IRendererHardware.h"
#include "RHI/VulkanRHI/VulkanRenderer.h"
#include "RHI/OpenGLRHI/OpenGLRenderer.h"

#include "Camera.h"

namespace Core
{
	class Application
	{
	private:
		Window m_Window;

		static inline Core::IRendererHardware* m_Renderer = nullptr;

		static inline RendererType m_RendererType = RendererType::VULKAN;

	public:
		static inline LowRenderer::Camera appCamera;

		///////////////////////////////////////////////////////////////////////

		/// Application class constructors

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Application default constructor
		/// </summary>
		Application() = default;

		/// <summary>
		/// Application default destructor
		/// </summary>
		~Application() = default;

		///////////////////////////////////////////////////////////////////////

		/// Getters and setters
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Window getter
		/// </summary>
		/// <returns></returns>
		inline Window* GetWindow() { return &m_Window; }

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Initialize the application
		/// </summary>
		/// <returns></returns>
		const bool Initialize();

		/// <summary>
		/// Terminate the application
		/// </summary>
		/// <returns></returns>
		const bool Terminate();

		///////////////////////////////////////////////////////////////////////

		/// Update and Draw methods
		
		///////////////////////////////////////////////////////////////////////

		void Draw();
	};
}