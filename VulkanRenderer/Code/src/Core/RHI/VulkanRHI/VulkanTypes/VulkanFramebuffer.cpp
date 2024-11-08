#include "RHI/VulkanRHI/VulkanTypes/VulkanFramebuffer.h"

namespace Core
{
	const RHI_RESULT VulkanFramebuffer::CreateFramebuffer()
	{
		// Specifies the attachments for each frame buffer
		std::array<VkImageView, 2> attachments = {
			m_SwapChainImageViews[i],			// ColorBuffer
			m_DepthImageView					// Depth Buffer
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		// Specifies the renderpass compatible with the framebuffer
		framebufferInfo.renderPass = m_RenderPass;
		// Specifies the attachments
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = m_SwapChainExtent.width;
		framebufferInfo.height = m_SwapChainExtent.height;
		// Layer number - More than 1 for 3D stereoscopic app
		framebufferInfo.layers = 1;

		// Creates the frambuffer
		VkResult result = vkCreateFramebuffer(m_LogicalDevice, &framebufferInfo, nullptr, &m_FrameBuffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create Framebuffer, Error Code: %d", result);
		}
	}

	const RHI_RESULT VulkanFramebuffer::DestroyFramebuffer()
	{
		vkDestroyFramebuffer(m_LogicalDevice, m_FrameBuffer, nullptr);
		return RHI_RESULT();
	}
}