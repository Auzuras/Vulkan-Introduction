#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	const int MAX_FRAMES_IN_FLIGHT = 2;

	//int m_CurrentFrame = 0;

	class Window;
	class IDevice;
	class VulkanSwapChain;

	class ISwapChain
	{
	public:
		virtual ~ISwapChain() = default;

		virtual const RHI_RESULT CreateSwapChain(Window* _Window, IDevice* _Device) = 0;
		virtual const RHI_RESULT RecreateSwapChain(Window* _Window, IDevice* _Device) = 0;
		virtual const RHI_RESULT DestroySwapChain(IDevice* _Device) = 0;

		virtual VulkanSwapChain* CastToVulkan() = 0;
	};
}