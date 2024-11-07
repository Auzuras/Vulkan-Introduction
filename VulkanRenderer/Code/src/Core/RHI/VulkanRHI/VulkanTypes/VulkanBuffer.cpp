#include "RHI/VulkanRHI/VulkanTypes/VulkanBuffer.h"

namespace Core
{
	uint32_t VulkanBuffer::FindMemoryType(uint32_t _TypeFilter, VkMemoryPropertyFlags _Properties)
	{
		// Gets all memory properties from the device
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

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

	void VulkanBuffer::CopyBuffer(VkBuffer _SourceBuffer, VkBuffer _DestinationBuffer, VkDeviceSize _Size)
	{
		// Creates a temporary command buffer for transfer
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		// Buffer Copy infos
		VkBufferCopy copyRegion{};
		copyRegion.size = _Size;

		// Copies a buffer from a source buffer to a destination one
		vkCmdCopyBuffer(commandBuffer, _SourceBuffer, _DestinationBuffer, 1, &copyRegion);

		// Ends and destroys the buffer
		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanBuffer::CreateBuffer(VkDeviceSize _Size, VkBufferUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory)
	{
		// Buffer infos
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _Size;
		bufferInfo.usage = _Usage;
		// Same as for the image view we can choose to share the buffer between multiple queues or keep it for one which is the case here
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		// Creates the buffer
		VkResult result = vkCreateBuffer(m_LogicalDevice, &bufferInfo, nullptr, &_Buffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create vertex buffer, Error Code %d", result);
		}

		// Get the memory requirements to allocate the memory
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_LogicalDevice, _Buffer, &memRequirements);

		// Memory allocation infos
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, _Properties);

		// Allocates the memory
		result = vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &_BufferMemory);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate vertex buffer memory, Error Code: %d", result);
		}

		// Bind the buffer with allocated emplacement
		vkBindBufferMemory(m_LogicalDevice, _Buffer, _BufferMemory, 0);
	}

	void VulkanBuffer::CreateVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Creates a transfer buffer
		// VK_BUFFER_USAGE_TRANSFER_SRC_BIT specifies that the buffer can be used as a source buffer and be copied in the memory
		// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT specifies that the memory is the same CPU and GPU side so easily accessible by both but not optimized
		// This is why we are using staging buffer and we copy it in the VRAM
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Maps the data into the GPU Memory and copy the data into the memory
		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Creates the vertex buffer
		// VK_BUFFER_USAGE_TRANSFER_DST_BIT specifies that the buffer can only receive data from memory of the GPU and that it is a VERTEX_BUFFER
		// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT specifies that the memory is only for GPU and optimized for GPU
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		// Copies the data from the staging buffer to the vertex buffer
		CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

		// Destroys the stagging buffer and free the memory
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
	}

	void VulkanBuffer::CreateIndexBuffer()
	{
		// Same idea as the vertex buffer so please check the CreateVertexBuffer for more information

		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Creates a transfer buffer
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Maps the data into the GPU Memory and copy the data into the memory
		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Creates the index buffer
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

		// Copies the data from the staging buffer to the index buffer
		CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);

		// Destroys the stagging buffer and free the memory
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
	}
}