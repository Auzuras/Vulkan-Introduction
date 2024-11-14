#include "Application.h"

namespace Core
{
	Application::Application()
	{}

	const bool Application::Initialize()
	{
		Debug::Log::OpenFile("Logs/LogFile.log");

		m_Window.Initialize(m_Renderer.GetRendererType());

		if (!m_Renderer.Initialize(&m_Window))
		{
			DEBUG_ERROR("Renderer failed to initialize");
			return false;
		}

		return true;
	}

	const bool Application::Terminate()
	{
		bool returnValue = true;

		if (!m_Renderer.Terminate())
		{
			DEBUG_ERROR("Renderer failed to terminate");
			returnValue = false;
		}

		Debug::Log::CloseFile();

		m_Window.Terminate();

		return returnValue;
	}

	void Application::Draw()
	{
		m_Renderer.StartFrame(&m_Window);

		// Draw

		m_Renderer.EndFrame();
	}
}