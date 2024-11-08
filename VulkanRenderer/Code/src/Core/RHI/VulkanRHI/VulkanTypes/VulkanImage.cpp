#include "RHI/VulkanRHI/VulkanTypes/VulkanImage.h"

namespace Core
{
	void VulkanImage::CreateImage(uint32_t _Width, uint32_t _Height, VkFormat _Format, VkImageTiling _Tiling, VkImageUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkImage& _Image, VkDeviceMemory& _ImageMemory)
	{
		// Omage infos
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		// Type of texture
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = _Width;
		imageInfo.extent.height = _Height;
		// Depth is 1 because it's a 2D texture
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = _Format;
		// How pixels are organized in the memory
		imageInfo.tiling = _Tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = _Usage;
		// For multi sampling
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		// Defines if the image can be used by multiple queues
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		// Creates the image
		VkResult result = vkCreateImage(m_LogicalDevice, &imageInfo, nullptr, &_Image);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create image, Error Code: %d", result);
		}

		// Gets the memory requirements for the texture
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_LogicalDevice, _Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		// Allocates the memory
		result = vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &_ImageMemory);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Filed to allocate image memory, Error Code: %d", result);
		}

		// Bind the memory with our image
		vkBindImageMemory(m_LogicalDevice, _Image, _ImageMemory, 0);
	}

	VkImageView VulkanImage::CreateImageView(VkImage _Image, VkFormat _Format, VkImageAspectFlags _AspectFlags)
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
		VkImageView imageView;
		VkResult result = vkCreateImageView(m_LogicalDevice, &viewInfo, nullptr, &imageView);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture image view, Error Code: %d", result);
			return nullptr;
		}

		return imageView;
	}

	void VulkanImage::TransitionImageLayout(VkImage _Image, VkFormat _Format, VkImageLayout _OldLayout, VkImageLayout _NewLayout)
	{
		// Starts recording command buffer
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		// Creates a memory barrier
		// Is here to garanty that we will not modify the memory while the GPU is using it or already writing in it
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER; \
			// Specifies the old layout and the new layout
			barrier.oldLayout = _OldLayout;
		barrier.newLayout = _NewLayout;
		// Used to change queue families
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		// Base data of the image
		barrier.image = _Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		// Default values
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = 0;

		// Creates src and dst stages that we have to defined
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		// For depth buffer
		if (_NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(_Format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}

		// Handles some transitions
		if (_OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (_OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (_OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			// Also for depth buffer
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			DEBUG_ERROR("Unsupported layout transition !");
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		// Ends recording the command buffer
		EndSingleTimeCommands(commandBuffer);
	}
}