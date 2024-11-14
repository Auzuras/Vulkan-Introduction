#pragma once

#include "RHI/RHITypes/IFramebuffer.h"
#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanFramebuffer
	{
	private:
		VkFramebuffer m_FrameBuffer;

	public:
		~VulkanFramebuffer() = default;

		const RHI_RESULT CreateFramebuffer(IDevice* _Device, IPipeline* _CompatiblePipeline, int _Width, int _Height, VkImageView& _ColorBufer, VkImageView& _DepthBuffer);
		const RHI_RESULT DestroyFramebuffer(IDevice* _Device);

		inline VkFramebuffer& GetFrameBuffer() { return m_FrameBuffer; }
	};
}