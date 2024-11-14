#include "RHI/VulkanRHI/VulkanTypes/VulkanMesh.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanBuffer.h"

#include "Renderer.h"

namespace Core
{
	RHI_RESULT VulkanMesh::CreateVertexBuffer(Core::IDevice* _Device, std::vector<Vertex>& _VerticesList)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		VkDeviceSize bufferSize = sizeof(_VerticesList[0]) * _VerticesList.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		m_VertexBuffer.CreateBuffer(_Device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Maps the data into the GPU Memory and copy the data into the memory
		void* data;
		vkMapMemory(device.GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, _VerticesList.data(), (size_t)bufferSize);
		vkUnmapMemory(device.GetLogicalDevice(), stagingBufferMemory);

		VkBuffer vertexBuffer = m_VertexBuffer.GetBuffer();
		VkDeviceMemory vertexBufferMemory = m_VertexBuffer.GetBufferMemory();

		// Creates the vertex buffer
		// VK_BUFFER_USAGE_TRANSFER_DST_BIT specifies that the buffer can only receive data from memory of the GPU and that it is a VERTEX_BUFFER
		// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT specifies that the memory is only for GPU and optimized for GPU
		m_VertexBuffer.CreateBuffer(_Device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

		// Copies the data from the staging buffer to the vertex buffer
		VulkanBuffer::CopyBuffer(&device, Core::Renderer::GetCommandAllocator()->CastToVulkan(), stagingBuffer, vertexBuffer, bufferSize);

		// Destroys the stagging buffer and free the memory
		vkDestroyBuffer(device.GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(device.GetLogicalDevice(), stagingBufferMemory, nullptr);

		return RHI_SUCCESS;
	}

	RHI_RESULT VulkanMesh::CreateIndexBuffer(Core::IDevice* _Device, std::vector<uint32_t>& _IndicesList)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		VkDeviceSize bufferSize = sizeof(_IndicesList[0]) * _IndicesList.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		m_VertexBuffer.CreateBuffer(_Device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Maps the data into the GPU Memory and copy the data into the memory
		void* data;
		vkMapMemory(device.GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, _IndicesList.data(), (size_t)bufferSize);
		vkUnmapMemory(device.GetLogicalDevice(), stagingBufferMemory);

		VkBuffer indexBuffer = m_IndexBuffer.GetBuffer();
		VkDeviceMemory indexBufferMemory = m_IndexBuffer.GetBufferMemory();

		// Creates the index buffer
		// VK_BUFFER_USAGE_TRANSFER_DST_BIT specifies that the buffer can only receive data from memory of the GPU and that it is a VERTEX_BUFFER
		// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT specifies that the memory is only for GPU and optimized for GPU
		m_VertexBuffer.CreateBuffer(_Device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

		// Copies the data from the staging buffer to the vertex buffer
		VulkanBuffer::CopyBuffer(&device, Core::Renderer::GetCommandAllocator()->CastToVulkan(), stagingBuffer, indexBuffer, bufferSize);

		// Destroys the stagging buffer and free the memory
		vkDestroyBuffer(device.GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(device.GetLogicalDevice(), stagingBufferMemory, nullptr);

		return RHI_SUCCESS;
	}

	RHI_RESULT VulkanMesh::DestroyBuffers(Core::IDevice* _Device)
	{
		m_VertexBuffer.DestroyBuffer(_Device);
		m_IndexBuffer.DestroyBuffer(_Device);

		return RHI_SUCCESS;
	}
}