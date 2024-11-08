#pragma once

#include "RHI/RHITypes/ICommandBuffer.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanCommandBuffer : public ICommandBuffer
	{
	private:
		// Commands
		VkCommandBuffer m_CommandBuffer;

	public:
		/// <summary>
		/// Creates all the command buffers
		/// </summary>
		void CreateCommandBuffers();

		/// <summary>
		/// Records all the commands for a command buffer
		/// </summary>
		/// <param name="_CommandBuffer">: Command buffer used </param>
		/// <param name="_ImageIndex">: Current image </param>
		void RecordDrawCommandBuffer(VkCommandBuffer _CommandBuffer, uint32_t _ImageIndex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_CommandBuffer"></param>
		void DrawCommandBuffer(VkCommandBuffer _CommandBuffer);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_CommandBuffer"></param>
		void EndDrawCommandBuffer(VkCommandBuffer _CommandBuffer);

		/// <summary>
		/// Creates a temporary command buffer for a single command
		/// </summary>
		/// <returns></returns>
		VkCommandBuffer BeginSingleTimeCommands();

		/// <summary>
		/// Ends and destroy a temporary command buffer for a single command
		/// </summary>
		/// <param name="_CommandBuffer">: Command buffer to destroy </param>
		void EndSingleTimeCommands(VkCommandBuffer _CommandBuffer);
	};
}