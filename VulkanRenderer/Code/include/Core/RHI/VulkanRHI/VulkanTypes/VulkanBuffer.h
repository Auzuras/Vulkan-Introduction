#pragma once

#include "RHI/RHITypes/IBuffer.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanImage;

	class VulkanBuffer : public IBuffer
	{
	private:

	public:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;

		~VulkanBuffer() override;

		const RHI_RESULT CreateBuffer(IDevice* _Device, BufferType _BufferType, size_t _BufferSize) override;
		const RHI_RESULT DestroyBuffer(IDevice* _Device) override;

		const RHI_RESULT CreateUniformBuffer(IDevice* _Device, size_t _BufferSize);

		inline VulkanBuffer* CastToVulkan() override { return this; }

		/// <summary>
		/// Creates and allocate a buffer in memory
		/// </summary>
		/// <param name="_Size">: Size of the buffer </param>
		/// <param name="_Usage">: Type of usage of the buffer </param>
		/// <param name="_Properties">: Buffer properties </param>
		/// <param name="_Buffer">: Buffer you want to create and store </param>
		/// <param name="_BufferMemory">: Buffer memory you want to allocate </param>
		void CreateBuffer(IDevice* _Device, VkDeviceSize _Size, VkBufferUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory);

		/// <summary>
		/// Copies a buffer from a source buffer to a destination buffer
		/// </summary>
		/// <param name="_SourceBuffer">: First buffer you want to copy </param>
		/// <param name="_DestinationBuffer">: Second buffer receiving the first one </param>
		/// <param name="_Size">: Size of the buffer </param>
		static void CopyBuffer(VulkanDevice* _Device, VulkanCommandAllocator* _CommandAllocator, VkBuffer _SourceBuffer, VkBuffer& _DestinationBuffer, VkDeviceSize& _Size);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Device"></param>
		/// <param name="_CommandAllocator"></param>
		/// <param name="_SourceBuffer"></param>
		/// <param name="_Image"></param>
		/// <param name="_Width"></param>
		/// <param name="_Height"></param>
		static void CopyBufferToImage(VulkanDevice* _Device, VulkanCommandAllocator* _CommandAllocator, VkBuffer _SourceBuffer, VulkanImage& _Image, uint32_t _Width, uint32_t _Height);

		/// <summary>
		/// Selects the best memory type for an allocation
		/// </summary>
		/// <param name="_TypeFilter">: Type of memory needed by a buffer </param>
		/// <param name="_Properties">: Properties needed for the buffer </param>
		/// <returns></returns>
		static uint32_t FindMemoryType(VkPhysicalDevice _PhysicalDevice, uint32_t _TypeFilter, VkMemoryPropertyFlags _Properties);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		inline VkBuffer GetBuffer() { return m_Buffer; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		inline VkDeviceMemory GetBufferMemory() { return m_BufferMemory; }

		void UpdateUBO(IDevice* _Device, void* _Data, size_t _DataSize) override;
	};
}