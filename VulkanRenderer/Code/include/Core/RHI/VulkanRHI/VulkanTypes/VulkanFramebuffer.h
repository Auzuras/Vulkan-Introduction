#pragma once

#include "RHI/RHITypes/IFramebuffer.h"
#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanFramebuffer : public IFrameBuffer
	{
	private:
		VkFramebuffer m_FrameBuffer;

	public:
		const RHI_RESULT CreateFramebuffer(IDevice* _Device, IPipeline* _CompatiblePipeline, int _Width, int _Height);
		const RHI_RESULT DestroyFramebuffer(IDevice* _Device);
	};
}