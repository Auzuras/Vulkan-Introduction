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
		const RHI_RESULT CreateFramebuffer(IDevice* _Device, IPipeline* _CompatiblePipeline, uint32_t _Width, uint32_t _Height);
		const RHI_RESULT DestroyFramebuffer(IDevice* _Device);
	};
}