#pragma once

#include "RHI/IRendererHardware.h"
#include "RHI/RHITypes.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class Renderer
	{
	private:
		static inline IRendererHardware* m_RHI = nullptr;
		static inline RendererType m_RendererType = VULKAN;

		IDevice* m_Device = nullptr;
		ISwapChain* m_SwapChain = nullptr;
		IPipeline* m_SimplePipeline = nullptr;
		static inline ICommandAllocator* m_CommandAllocator = nullptr;
		static inline IDescriptorAllocator* m_DescriptorAllocator = nullptr;

		std::vector<ICommandBuffer*> m_CommandBuffers;

		std::vector<ISemaphore*> m_ImageAvailableSemaphores;
		std::vector<ISemaphore*> m_RenderFinishedSemaphores;
		std::vector<IFence*> m_InFlightFramesFences;

		unsigned int m_CurrentFrame = 0;
		unsigned int imageIndex = 0;

	public:
		const int MAX_FRAMES_IN_FLIGHT = 2;

		static inline IRendererHardware* GetRHI() { return m_RHI; }
		static inline RendererType GetRHIType() { return m_RendererType; }
		static inline ICommandAllocator* GetCommandAllocator() { return m_CommandAllocator; }
		static inline IDescriptorAllocator* GetDescriptorAllocator() { return m_DescriptorAllocator; }

		Renderer() = default;

		inline RendererType GetRendererType() const { return m_RendererType; }

		const bool Initialize(Window* _Window);
		void CreateSimplePipeline();

		void StartFrame(Window* _Window);
		void EndFrame(Window* _Window);

		void SetupTexturedModelPass();
		void TexturedModelPass(IMesh* _Mesh);
		void FinishTexturedModelPass();

		const bool Terminate();
	};
}