#pragma once

// Log.hpp contains Windows.h, needs to be included first to avoid macro redefinition
#include "Debug/Log.h"

#include "RHI/RHI.h"

namespace Core
{
	enum RHIType
	{
		VULKAN
	};

	class Application
	{
	private:
		const uint32_t m_Width = 800;
		const uint32_t m_Height = 600;
		const char* m_WindowName = "DefaultName";

		GLFWwindow* m_Window = nullptr;
		static inline Core::RHI* m_RHI = nullptr;

		RHIType m_RHIType = RHIType::VULKAN;

	public:

		///////////////////////////////////////////////////////////////////////

		/// Application class constructors

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Application default constructot
		/// </summary>
		Application() = default;

		/// <summary>
		/// Application constructor
		/// </summary>
		/// <param name="_Width">: Width of the window </param>
		/// <param name="_Height">: Height of the window </param>
		/// <param name="_WindowName">: Name of the window </param>
		/// <param name="_RHIType">: Type of renderer </param>
		Application(const uint32_t _Width, const uint32_t _Height, const char* _WindowName, RHIType _RHIType);

		/// <summary>
		/// Application constructor
		/// </summary>
		/// <param name="_Width">: Width of the window </param>
		/// <param name="_Height">: Height of the window </param>
		Application(const uint32_t _Width, const uint32_t _Height);

		/// <summary>
		/// Application default destructor
		/// </summary>
		~Application() = default;

		///////////////////////////////////////////////////////////////////////

		/// Getters and setters
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Window width getter
		/// </summary>
		/// <returns></returns>
		inline int GetWindowWidth() { return m_Width; }

		/// <summary>
		/// Window height getter
		/// </summary>
		/// <returns></returns>
		inline int GetWindowHeight() { return m_Height; }

		/// <summary>
		/// Window name getter
		/// </summary>
		/// <returns></returns>
		inline const char* GetWindowName() { return m_WindowName; }

		/// <summary>
		/// Window getter
		/// </summary>
		/// <returns></returns>
		inline GLFWwindow* GetWindow() { return m_Window; }

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