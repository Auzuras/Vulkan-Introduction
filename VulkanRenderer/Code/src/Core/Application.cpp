#include "Application.h"

namespace Core
{
	const bool Application::Initialize()
	{
		m_Window.Initialize(m_RendererType);

		switch (m_RendererType)
		{
		case Core::OPENGL:
			m_Renderer = new Core::OpenGLRenderer;
			break;
		case Core::VULKAN: default:
			m_Renderer = new Core::VulkanRenderer;
			break;
		case Core::DIRECTX:
			// TODO: DirectX renderer
			break;
		}

		Debug::Log::OpenFile("Logs/LogFile.log");

		if (!m_Renderer)
		{
			DEBUG_ERROR("RHI is null, cancelling application initialization");
			return false;
		}

		if (!m_Renderer->Initialize(&m_Window))
		{
			DEBUG_ERROR("Application failed to initialize");
			return false;
		}

		return true;
	}

	const bool Application::Terminate()
	{
		bool returnValue = true;

		if (!m_Renderer->Terminate())
		{
			DEBUG_ERROR("Aplication failed to terminate");
			returnValue = false;
		}

		delete m_Renderer;
		m_Renderer = nullptr;

		Debug::Log::CloseFile();

		//m_WindowDos.Terminate();

		m_Window.Terminate();

		return returnValue;
	}

	void Application::Draw()
	{
		m_Renderer->DrawFrame(&m_Window);
	}
}