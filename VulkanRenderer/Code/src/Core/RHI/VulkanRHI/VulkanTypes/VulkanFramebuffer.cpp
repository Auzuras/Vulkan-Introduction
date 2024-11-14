#include "RHI/VulkanRHI/VulkanTypes/VulkanFramebuffer.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanPipeline.h"

namespace Core
{
	const RHI_RESULT VulkanFramebuffer::CreateFramebuffer(IDevice* _Device, IPipeline* _CompatiblePipeline, int _Width, int _Height, VkImageView& _ColorBufer, VkImageView& _DepthBuffer)
	{
		VulkanDevice device = *_Device->CastToVulkan();
		VulkanPipeline pipeline = *_CompatiblePipeline->CastToVulkan();

		// Specifies the attachments for each frame buffer
		std::array<VkImageView, 2> attachments = {
			_ColorBufer,					// ColorBuffer
			_DepthBuffer					// Depth Buffer
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		// Specifies the renderpass compatible with the framebuffer
		framebufferInfo.renderPass = pipeline.GetRenderPass();
		// Specifies the attachments
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = static_cast<uint32_t>(_Width);
		framebufferInfo.height = static_cast<uint32_t>(_Height);
		// Layer number - More than 1 for 3D stereoscopic app
		framebufferInfo.layers = 1;

		// Creates the frambuffer
		VkResult result = vkCreateFramebuffer(device.GetLogicalDevice(), &framebufferInfo, nullptr, &m_FrameBuffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create Framebuffer, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	const RHI_RESULT VulkanFramebuffer::DestroyFramebuffer(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroyFramebuffer(device.GetLogicalDevice(), m_FrameBuffer, nullptr);

		return RHI_SUCCESS;
	}
}