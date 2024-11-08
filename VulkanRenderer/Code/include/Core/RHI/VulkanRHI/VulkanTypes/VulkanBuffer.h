#pragma once

#include "RHI/RHITypes/IBuffer.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanBuffer : public IBuffer
	{
	private:

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;

		/// <summary>
		/// Selects the best memory type for an allocation
		/// </summary>
		/// <param name="_TypeFilter">: Type of memory needed by a buffer </param>
		/// <param name="_Properties">: Properties needed for the buffer </param>
		/// <returns></returns>
		uint32_t FindMemoryType(uint32_t _TypeFilter, VkMemoryPropertyFlags _Properties);

		/// <summary>
		/// Copies a buffer from a source buffer to a destination buffer
		/// </summary>
		/// <param name="_SourceBuffer">: First buffer you want to copy </param>
		/// <param name="_DestinationBuffer">: Second buffer receiving the first one </param>
		/// <param name="_Size">: Size of the buffer </param>
		void CopyBuffer(VkBuffer _SourceBuffer, VkBuffer _DestinationBuffer, VkDeviceSize _Size);

	public:

		/// <summary>
		/// Creates and allocate a buffer in memory
		/// </summary>
		/// <param name="_Size">: Size of the buffer </param>
		/// <param name="_Usage">: Type of usage of the buffer </param>
		/// <param name="_Properties">: Buffer properties </param>
		/// <param name="_Buffer">: Buffer you want to create and store </param>
		/// <param name="_BufferMemory">: Buffer memory you want to allocate </param>
		void CreateBuffer(VkDeviceSize _Size, VkBufferUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory);

		/// <summary>
		/// Creates a vertex buffer
		/// </summary>
		void CreateVertexBuffer();

		/// <summary>
		/// Creates an index buffer
		/// </summary>
		void CreateIndexBuffer();

		void DestroyBuffer();
	};
}