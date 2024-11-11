#include "Renderer.h"

namespace Core
{
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

		CreateSimplePipeline();

		m_CommandAllocator = m_RHI->InstantiateCommandAllocator(m_Device);

		return true;
	}

	void Renderer::CreateSimplePipeline()
	{
		IShader* vertShader = m_RHI->CreateShader();
		vertShader->Load(m_Device, "Assets/Shaders/HelloTriangle.vert");

		IShader* fragShader = m_RHI->CreateShader();
		vertShader->Load(m_Device, "Assets/Shaders/HelloTriangle.frag");

		PipelineShaderInfos vert;
		vert.shader = vertShader;
		vert.shaderType = VERTEX;
		vert.functionEntry = "main";

		PipelineShaderInfos frag;
		frag.shader = fragShader;
		frag.shaderType = FRAGMENT;
		frag.functionEntry = "main";

		std::vector<PipelineShaderInfos> shadersInfos = { vert, frag };

		m_SimplePipeline = m_RHI->InstantiatePipeline(m_Device, m_SwapChain, shadersInfos);
	}

	const bool Renderer::Terminate()
	{
		m_RHI->DestroyCommandAllocator(m_CommandAllocator, m_Device);

		m_RHI->DestroyPipeline(m_SimplePipeline, m_Device);

		m_RHI->DestroySwapChain(m_SwapChain, m_Device);

		m_RHI->DestroyDevice(m_Device);

		delete m_RHI;
		m_RHI = nullptr;

		return true;
	}
}