#include "RHI/VulkanRHI/VulkanTypes/VulkanBuffer.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandBuffer.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanImage.h"

namespace Core
{
	uint32_t VulkanBuffer::FindMemoryType(VkPhysicalDevice _PhysicalDevice, uint32_t _TypeFilter, VkMemoryPropertyFlags _Properties)
	{
		// Gets all memory properties from the device
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(_PhysicalDevice, &memProperties);

		// Loop going through all memory types
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			// _TypeFilter & (1 << i) checks if the i byte is equal to the one in _TypeFilter 
			// if its the case it means that the memory type correspond to the _TypeFilter
			// The second condition checks if all _Properties are present in memProperties.memoryTypes[i].propertyFlags
			if (_TypeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & _Properties) == _Properties)
			{
				return i;
			}
		}

		DEBUG_ERROR("Failed to find suitable memory type");
		return 0;
	}

	void VulkanBuffer::CopyBuffer(VulkanDevice* _Device, VulkanCommandAllocator* _CommandAllocator, VkBuffer _SourceBuffer, VkBuffer& _DestinationBuffer, VkDeviceSize& _Size)
	{
		// Creates a temporary command buffer for transfer
		VkCommandBuffer commandBuffer = VulkanCommandBuffer::BeginSingleTimeCommands(_Device, _CommandAllocator);

		// Buffer Copy infos
		VkBufferCopy copyRegion{};
		copyRegion.size = _Size;

		// Copies a buffer from a source buffer to a destination one
		vkCmdCopyBuffer(commandBuffer, _SourceBuffer, _DestinationBuffer, 1, &copyRegion);

		// Ends and destroys the buffer
		VulkanCommandBuffer::EndSingleTimeCommands(_Device, _CommandAllocator, commandBuffer);
	}

	void VulkanBuffer::CopyBufferToImage(VulkanDevice* _Device, VulkanCommandAllocator* _CommandAllocator, VkBuffer _SourceBuffer, VulkanImage& _Image, uint32_t _Width, uint32_t _Height)
	{
		// Creates a temporary command buffer for transfer
		VkCommandBuffer commandBuffer = VulkanCommandBuffer::BeginSingleTimeCommands(_Device, _CommandAllocator);

		// Creates the region to copy
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { _Width, _Height, 1 };

		// Copies the buffer to the image
		vkCmdCopyBufferToImage(commandBuffer, _SourceBuffer, _Image.GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		// Ends recording the command buffer
		VulkanCommandBuffer::EndSingleTimeCommands(_Device, _CommandAllocator, commandBuffer);
	}

	void VulkanBuffer::CreateBuffer(IDevice* _Device, VkDeviceSize _Size, VkBufferUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Buffer infos
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _Size;
		bufferInfo.usage = _Usage;
		// Same as for the image view we can choose to share the buffer between multiple queues or keep it for one which is the case here
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		// Creates the buffer
		VkResult result = vkCreateBuffer(device.GetLogicalDevice(), &bufferInfo, nullptr, &_Buffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create vertex buffer, Error Code %d", result);
		}

		// Get the memory requirements to allocate the memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.GetLogicalDevice(), _Buffer, &memRequirements);

		// Memory allocation infos
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(device.GetPhysicalDevice(), memRequirements.memoryTypeBits, _Properties);

		// Allocates the memory
		result = vkAllocateMemory(device.GetLogicalDevice(), &allocInfo, nullptr, &_BufferMemory);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate vertex buffer memory, Error Code: %d", result);
		}

		// Bind the buffer with allocated emplacement
		vkBindBufferMemory(device.GetLogicalDevice(), _Buffer, _BufferMemory, 0);
	}

	void VulkanBuffer::DestroyBuffer(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroyBuffer(device.GetLogicalDevice(), m_Buffer, nullptr);
		vkFreeMemory(device.GetLogicalDevice(), m_BufferMemory, nullptr);
	}
}