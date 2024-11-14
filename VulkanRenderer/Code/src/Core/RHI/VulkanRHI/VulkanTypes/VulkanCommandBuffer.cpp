#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandBuffer.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandAllocator.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanMesh.h"

namespace Core
{
	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer _CommandBuffer)
		:m_CommandBuffer(_CommandBuffer)
	{}

	VkCommandBuffer VulkanCommandBuffer::BeginSingleTimeCommands(VulkanDevice* _Device, VulkanCommandAllocator* _CommandAllocator)
	{
		// Command buffer allocation infos
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _CommandAllocator->GetCommandAllocator();
		allocInfo.commandBufferCount = 1;

		//// Allocates the command buffer
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(_Device->GetLogicalDevice(), &allocInfo, &commandBuffer);

		//// Begin info specifing VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT which means the command buffer will be used for a single command
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		//// Start recording the command
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanCommandBuffer::EndSingleTimeCommands(VulkanDevice* _Device, VulkanCommandAllocator* _CommandAllocator, VkCommandBuffer _CommandBuffer)
	{
		// Ends recording the command
		vkEndCommandBuffer(_CommandBuffer);

		// Submit infos
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_CommandBuffer;

		// Sumbit the queue
		vkQueueSubmit(_Device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);

		// Wait for the actions to finish before destroying it
		vkQueueWaitIdle(_Device->GetGraphicsQueue());

		// Free and destroy the command buffer
		vkFreeCommandBuffers(_Device->GetLogicalDevice(), _CommandAllocator->GetCommandAllocator(), 1, &_CommandBuffer);
	}

	void VulkanCommandBuffer::ResetCommandBuffer()
	{
		vkResetCommandBuffer(m_CommandBuffer, 0);
	}

	const RHI_RESULT VulkanCommandBuffer::StartRecordingCommandBuffer()
	{
		// Command buffer begin recording infos
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		// Specifies the behavior of the command buffer (0 = None)
		beginInfo.flags = 0;
		// Only for secondary command buffers
		beginInfo.pInheritanceInfo = nullptr;

		// Start the command buffer
		VkResult result = vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to begin recording command buffer, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	const RHI_RESULT VulkanCommandBuffer::StopRecordingCommandBuffer() const
	{
		VkResult result = vkEndCommandBuffer(m_CommandBuffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to record command buffer, Error Code %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	void VulkanCommandBuffer::BindVertexBuffer(IMesh* _Mesh) const
	{
		VulkanMesh mesh = *_Mesh->CastToVulkan();

		VkBuffer vertexBuffers[] = { mesh.GetVertexBuffer().GetBuffer() };
		VkDeviceSize offsets[] = { 0 };

		// Binds the vertex buffer
		vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, vertexBuffers, offsets);
	}

	void VulkanCommandBuffer::BindIndexBuffer(IMesh* _Mesh) const
	{
		VkBuffer buffer = _Mesh->CastToVulkan()->GetIndexBuffer().GetBuffer();

		// Binds the index buffer
		vkCmdBindIndexBuffer(m_CommandBuffer, buffer, 0, VK_INDEX_TYPE_UINT32);

	}

	void VulkanCommandBuffer::DrawIndexed(IMesh* _Mesh) const
	{
		// Draws the vertex buffer with indices
		vkCmdDrawIndexed(m_CommandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	}

	void VulkanCommandBuffer::EndRenderPass() const
	{
		// Finish the render pass
		vkCmdEndRenderPass(m_CommandBuffer);
	}
}