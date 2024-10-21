#include "Application/Application.h"

#include "RHI/VulkanWrapper.h"

namespace Core
{
	Application::Application(const uint32_t _Width, const uint32_t _Height, const char* _WindowName, RHIType _RHIType)
		:m_Width(_Width), m_Height(_Height), m_WindowName(_WindowName), m_RHIType(_RHIType)
	{}

	Application::Application(const uint32_t _Width, const uint32_t _Height)
		:m_Width(_Width), m_Height(_Height)
	{}

	const bool Application::Initialize()
	{
		// TODO: Wrapper class for window

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName, nullptr, nullptr);

		switch (m_RHIType)
		{
		case Core::VULKAN: default:
			m_RHI = new Core::VulkanWrapper;
			break;
		}

		Debug::Log::OpenFile("Logs/LogFile.log");

		if (!m_RHI)
		{
			DEBUG_ERROR("RHI is null, cancelling application initialization");
			return false;
		}

		if (!m_RHI->Initialize())
		{
			DEBUG_ERROR("Application failed to initialize");
			return false;
		}

		return true;
	}

	const bool Application::Terminate()
	{
		bool returnValue = true;

		if (!m_RHI->Terminate())
		{
			DEBUG_ERROR("Aplication failed to terminate");
			returnValue = false;
		}

		delete m_RHI;

		Debug::Log::CloseFile();

		glfwDestroyWindow(m_Window);
		glfwTerminate();

		return returnValue;
	}
}