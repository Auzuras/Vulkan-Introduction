#include "Renderer.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	const bool Renderer::Initialize(Window* _Window)
	{
		switch (m_RendererType)
		{
		case Core::RHI_OPENGL:
			break;
		case Core::RHI_VULKAN: default:
			m_RHI = new VulkanRenderer;
			break;
		case Core::RHI_DIRECTX:
			break;
		}

		// Initialization of what we need

		m_Device = m_RHI->InstanciateDevice(_Window);

		m_SwapChain = m_RHI->InstantiateSwapChain(_Window, m_Device);

		CreateSimplePipeline();

		m_CommandAllocator = m_RHI->InstantiateCommandAllocator(m_Device);

		m_CommandBuffers = m_CommandAllocator->CreateCommandBuffers(m_Device, MAX_FRAMES_IN_FLIGHT);

		m_DescriptorAllocator = m_RHI->InstantiateDescriptorAllocator(m_Device, m_SwapChain);

		m_SwapChain->RecreateSwapChain(_Window, m_Device, m_SimplePipeline);

		m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFramesFences.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			m_ImageAvailableSemaphores[i] = m_RHI->InstantiateSemaphore(m_Device);
			m_RenderFinishedSemaphores[i] = m_RHI->InstantiateSemaphore(m_Device);
			m_InFlightFramesFences[i] = m_RHI->InstantiateFence(m_Device);
		}

		mesh = m_RHI->CreateMesh();
		mesh->Load(m_Device, "Assets/Meshes/viking_room.obj");

		texture = m_RHI->CreateTexture();
		texture->Load(m_Device, "Assets/Textures/viking_room.png");

		mcMesh = m_RHI->CreateMesh();
		mcMesh->Load(m_Device, "Assets/Meshes/minecraft.obj");

		mctexture = m_RHI->CreateTexture();
		mctexture->Load(m_Device, "Assets/Textures/minecraft.png");

		model = LowRenderer::Model(mesh, texture);
		mcModel = LowRenderer::Model(mcMesh, mctexture);

		return true;
	}

	void Renderer::CreateSimplePipeline()
	{
		IShader* vertShader = m_RHI->CreateShader();
		vertShader->Load(m_Device, "Assets/Shaders/BasicShader.vert");

		IShader* fragShader = m_RHI->CreateShader();
		fragShader->Load(m_Device, "Assets/Shaders/BasicShader.frag");

		PipelineShaderInfos vert;
		vert.shader = vertShader;
		vert.shaderType = RHI_VERTEX;
		vert.functionEntry = "main";

		PipelineShaderInfos frag;
		frag.shader = fragShader;
		frag.shaderType = RHI_FRAGMENT;
		frag.functionEntry = "main";

		std::vector<PipelineShaderInfos> shadersInfos = { vert, frag };

		m_SimplePipeline = m_RHI->InstantiatePipeline(m_Device, m_SwapChain, shadersInfos);

		fragShader->Unload(m_Device);
		vertShader->Unload(m_Device);

		m_RHI->DestroyShader(vertShader);
		m_RHI->DestroyShader(fragShader);
	}

	void Renderer::StartFrame(Window* _Window, LowRenderer::Camera* _Camera)
	{
		m_InFlightFramesFences[m_CurrentFrame]->WaitFence(m_Device, UINT64_MAX);
		m_InFlightFramesFences[m_CurrentFrame]->ResetFence(m_Device);
		
		m_SwapChain->AcquireNextImage(_Window, m_Device, m_SimplePipeline, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], imageIndex);

		m_CommandBuffers[m_CurrentFrame]->ResetCommandBuffer();

		// Setup the command buffer
		m_CommandBuffers[m_CurrentFrame]->StartRecordingCommandBuffer();

		LowRenderer::CameraData data;
		data.viewMatrix = _Camera->viewMatrix.Transpose();
		data.projectionMatrix = _Camera->projectionMatrix.Transpose();

		_Camera->GetUBO(m_CurrentFrame)->UpdateUBO(m_Device, &data, sizeof(data));
	}

	void Renderer::EndFrame(Window* _Window)
	{
		m_SwapChain->SubmitGraphicsQueue(m_Device, m_CommandBuffers[m_CurrentFrame], m_ImageAvailableSemaphores[m_CurrentFrame], m_RenderFinishedSemaphores[m_CurrentFrame], m_InFlightFramesFences[m_CurrentFrame]);
		m_SwapChain->SubmitPresentQueue(_Window, m_Device, m_SimplePipeline, m_RenderFinishedSemaphores[m_CurrentFrame], imageIndex);

		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::SetupTexturedModelPass()
	{
		m_CommandBuffers[m_CurrentFrame]->StartRenderPass(m_SimplePipeline, m_SwapChain, imageIndex, Math::Vector4(0.1f, 0.3f, 1.f, 1.f));
		m_CommandBuffers[m_CurrentFrame]->BindPipeline(m_SimplePipeline);
		m_CommandBuffers[m_CurrentFrame]->SetViewport(Math::Vector2::zero, m_SwapChain, 0.f, 1.f);
		m_CommandBuffers[m_CurrentFrame]->SetScissor(Math::Vector2::zero, m_SwapChain);
	}

	void Renderer::TexturedModelPass(LowRenderer::Camera* _Camera, LowRenderer::Model* _Model)
	{
		Math::Matrix4 localTRS = _Model->m_Transform.m_LocalTRS;

		_Model->GetUBO(m_CurrentFrame)->UpdateUBO(m_Device, &localTRS, sizeof(localTRS));

		m_CommandBuffers[m_CurrentFrame]->BindDescriptorSet(m_SimplePipeline, _Model->GetDescriptor(m_CurrentFrame), 0); // TRS
		m_CommandBuffers[m_CurrentFrame]->BindDescriptorSet(m_SimplePipeline, _Camera->GetDescriptor(m_CurrentFrame), 1);// Camera
		m_CommandBuffers[m_CurrentFrame]->BindDescriptorSet(m_SimplePipeline, _Model->GetTexture()->GetDescriptor(m_CurrentFrame), 2); // Texture

		m_CommandBuffers[m_CurrentFrame]->BindVertexBuffer(_Model->GetMesh());
		m_CommandBuffers[m_CurrentFrame]->BindIndexBuffer(_Model->GetMesh());

		m_CommandBuffers[m_CurrentFrame]->DrawIndexed(_Model->GetMesh());
	}

	void Renderer::FinishTexturedModelPass()
	{
		m_CommandBuffers[m_CurrentFrame]->EndRenderPass();
		m_CommandBuffers[m_CurrentFrame]->StopRecordingCommandBuffer();
	}

	const bool Renderer::Terminate(LowRenderer::Camera* _Camera)
	{
		m_Device->WaitDeviceIdle();
		
		_Camera->DeleteDescriptors();

		model.DestroyDescriptors();
		mcModel.DestroyDescriptors();

		mcMesh->Unload(m_Device);

		m_RHI->DestroyMesh(mcMesh);

		mctexture->Unload(m_Device);

		m_RHI->DestroyTexture(mctexture);

		mesh->Unload(m_Device);

		m_RHI->DestroyMesh(mesh);

		texture->Unload(m_Device);

		m_RHI->DestroyTexture(texture);

		m_RHI->DestroySwapChain(m_SwapChain, m_Device);

		m_RHI->DestroyPipeline(m_SimplePipeline, m_Device);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			m_RHI->DestroySemaphore(m_ImageAvailableSemaphores[i], m_Device);
			m_RHI->DestroySemaphore(m_RenderFinishedSemaphores[i], m_Device);
			m_RHI->DestroyFence(m_InFlightFramesFences[i], m_Device);
		}

		m_RHI->DestroyDescriptorAllocator(m_DescriptorAllocator, m_Device);

		m_RHI->DestroyCommandBuffers(m_Device, m_CommandBuffers);

		m_RHI->DestroyCommandAllocator(m_CommandAllocator, m_Device);

		m_RHI->DestroyDevice(m_Device);

		delete m_RHI;
		m_RHI = nullptr;

		return true;
	}
}