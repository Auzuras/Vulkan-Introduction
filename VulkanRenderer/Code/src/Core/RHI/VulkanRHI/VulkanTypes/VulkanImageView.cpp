#include "RHI/VulkanRHI/VulkanTypes/VulkanImageView.h"

namespace Core
{
	const RHI_RESULT VulkanImageView::CreateImageView(VkDevice _Device, VkImage _Image, VkFormat _Format, VkImageAspectFlags _AspectFlags)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _Image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = _Format;
		viewInfo.subresourceRange.aspectMask = _AspectFlags;
		// Mipmap levels
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		// baseArrayLayer - layerCount is the number of layers an image possess - only usefull for 3D stereoscopic applications
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		// Creates the image view
		VkResult result = vkCreateImageView(_Device, &viewInfo, nullptr, &m_ImageView);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture image view, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	const RHI_RESULT VulkanImageView::DestroyImageView(VkDevice _Device)
	{
		vkDestroyImageView(_Device, m_ImageView, nullptr);

		return RHI_SUCCESS;
	}
}