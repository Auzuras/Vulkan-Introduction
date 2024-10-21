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

		Application() = default;
		Application(const uint32_t _Width, const uint32_t _Height, const char* _WindowName, RHIType _RHIType);
		Application(const uint32_t _Width, const uint32_t _Height);
		~Application() = default;

		///////////////////////////////////////////////////////////////////////

		/// Getters and setters
		
		///////////////////////////////////////////////////////////////////////

		inline int GetWindowWidth() { return m_Width; }
		inline int GetWindowHeight() { return m_Height; }
		inline const char* GetWindowName() { return m_WindowName; }
		inline GLFWwindow* GetWindow() { return m_Window; }

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		const bool Initialize();
		const bool Terminate();
	};
}