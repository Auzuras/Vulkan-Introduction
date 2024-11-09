#include "Renderer.h"

namespace Core
{
	Renderer::Renderer(RendererType _RendererType)
		:m_RendererType(_RendererType)
	{}

	const bool Renderer::Initialize(Window* _Window)
	{
		switch (m_RendererType)
		{
		case Core::OPENGL:
			break;
		case Core::VULKAN: default:
			m_RHI = new VulkanRenderer;
			break;
		case Core::DIRECTX:
			break;
		}

		// Initialization of what we need

		m_Device = m_RHI->InstanciateDevice(_Window);

		m_SwapChain = m_RHI->InstantiateSwapChain(_Window, m_Device);

		m_SimplePipeline = m_RHI->InstantiatePipeline(m_Device);

		return true;
	}

	const bool Renderer::Terminate()
	{
		m_RHI->DestroyPipeline(m_SimplePipeline, m_Device);

		m_RHI->DestroySwapChain(m_SwapChain, m_Device);

		m_RHI->DestroyDevice(m_Device);

		delete m_RHI;
		m_RHI = nullptr;

		return true;
	}
}