#pragma once

#include "RHI/IRendererHardware.h"
#include "RHI/RHITypes.h"
#include "Model.h"
#include "Camera.h"

namespace Core
{
	class Renderer
	{
	private:
		// Lot of static here, I know it's not the solution
		// Because I dont know how to organize some classes I used it
		// And beacause it is the last day and no more time for architecture

		static inline IRendererHardware* m_RHI = nullptr;
		static inline RendererType m_RendererType = RHI_VULKAN;

		static inline IDevice* m_Device = nullptr;
		ISwapChain* m_SwapChain = nullptr;
		static inline IPipeline* m_SimplePipeline = nullptr;
		static inline ICommandAllocator* m_CommandAllocator = nullptr;
		static inline IDescriptorAllocator* m_DescriptorAllocator = nullptr;

		std::vector<ICommandBuffer*> m_CommandBuffers;

		std::vector<ISemaphore*> m_ImageAvailableSemaphores;
		std::vector<ISemaphore*> m_RenderFinishedSemaphores;
		std::vector<IFence*> m_InFlightFramesFences;

		unsigned int m_CurrentFrame = 0;
		unsigned int imageIndex = 0;

	public:
		IMesh* mesh = nullptr;
		ITexture* texture = nullptr;
		IMesh* mcMesh = nullptr;
		ITexture* mctexture = nullptr;
		static inline LowRenderer::Model model;
		static inline LowRenderer::Model mcModel;

		static inline const int MAX_FRAMES_IN_FLIGHT = 2;

		static inline IRendererHardware* GetRHI() { return m_RHI; }
		static inline RendererType GetRHIType() { return m_RendererType; }
		static inline IDevice* GetDevice() { return m_Device; }
		static inline ICommandAllocator* GetCommandAllocator() { return m_CommandAllocator; }
		static inline IDescriptorAllocator* GetDescriptorAllocator() { return m_DescriptorAllocator; }
		static inline IPipeline* GetPipeline() { return m_SimplePipeline; }

		Renderer() = default;

		inline RendererType GetRendererType() const { return m_RendererType; }

		const bool Initialize(Window* _Window);
		void CreateSimplePipeline();

		void StartFrame(Window* _Window, LowRenderer::Camera* _Camera);
		void EndFrame(Window* _Window);

		void SetupTexturedModelPass();
		void TexturedModelPass(LowRenderer::Camera* _Camera, LowRenderer::Model* _Model);
		void FinishTexturedModelPass();

		const bool Terminate(LowRenderer::Camera* _Camera);
	};
}