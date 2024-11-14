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

	public:
		static inline IRendererHardware* GetRHI() { return m_RHI; }
		static inline RendererType GetRHIType() { return m_RendererType; }
		static inline ICommandAllocator* GetCommandAllocator() { return m_CommandAllocator; }

		Renderer() = default;

		inline RendererType GetRendererType() const { return m_RendererType; }

		const bool Initialize(Window* _Window);
		void CreateSimplePipeline();
		const bool Terminate();
	};
}