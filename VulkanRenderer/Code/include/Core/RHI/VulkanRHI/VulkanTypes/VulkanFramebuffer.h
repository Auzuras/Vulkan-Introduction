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
		const RHI_RESULT CreateFramebuffer();
		const RHI_RESULT DestroyFramebuffer();
	};
}