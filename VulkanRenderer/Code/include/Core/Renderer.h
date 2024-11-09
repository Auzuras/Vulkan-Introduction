#pragma once

#include "RHI/IRendererHardware.h"
#include "RHI/RHITypes.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class Renderer
	{
	private:
		IRendererHardware* m_RHI = nullptr;
		RendererType m_RendererType = VULKAN;

		IDevice* m_Device = nullptr;
		ISwapChain* m_SwapChain = nullptr;
		IPipeline* m_SimplePipeline = nullptr;

	public:
		Renderer() = default;
		Renderer(RendererType _RendererType);

		inline RendererType GetRendererType() const { return m_RendererType; }

		const bool Initialize(Window* _Window);
		const bool Terminate();
	};
}