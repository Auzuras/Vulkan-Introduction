#pragma once

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
		int m_Width = 800;
		int m_Height = 600;
		const char* m_WindowName = "DefaultName";

		GLFWwindow* m_Window = nullptr;
		static inline Core::RHI* m_RHI = nullptr;

		RHIType m_RHIType = RHIType::VULKAN;

	public:

		///////////////////////////////////////////////////////////////////////

		/// Application class constructors

		///////////////////////////////////////////////////////////////////////

		Application() = default;
		Application(int _Width, int _Height, const char* _WindowName, RHIType _RHIType);
		Application(int _Width, int _Height);
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