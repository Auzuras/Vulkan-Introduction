#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class Window;
	class IDevice;
	class VulkanSwapChain;
	class IPipeline;
	class ISemaphore;
	class IFence;
	class ICommandBuffer;

	class ISwapChain
	{
	public:
		virtual ~ISwapChain() = default;

		bool framebufferResized;

		virtual const RHI_RESULT CreateSwapChain(Window* _Window, IDevice* _Device) = 0;
		virtual const RHI_RESULT RecreateSwapChain(Window* _Window, IDevice* _Device, IPipeline* _Pipeline) = 0;
		virtual const RHI_RESULT DestroySwapChain(IDevice* _Device) = 0;

		virtual VulkanSwapChain* CastToVulkan() = 0;

		virtual RHI_RESULT CreateSwapChainFramebuffers(IDevice* _Device, IPipeline* _Pipeline) = 0;
		virtual void AcquireNextImage(Window* _Window, IDevice* _Device, IPipeline* _Pipeline, unsigned int _Timeout, ISemaphore* _ImageAvailableSemaphore, unsigned int& _ImageIndex) = 0;

		virtual RHI_RESULT SubmitGraphicsQueue(IDevice* _Device, ICommandBuffer* _CommandBuffer, ISemaphore* _ImageAvailableSemaphore, ISemaphore* _RenderFinishSemaphore, IFence* _InFlightFence) = 0;
		virtual RHI_RESULT SubmitPresentQueue(Window* _Window, IDevice* _Device, IPipeline* _Pipeline, ISemaphore* _RenderFinishSemaphore, unsigned int _ImageIndex) = 0;
	};
}