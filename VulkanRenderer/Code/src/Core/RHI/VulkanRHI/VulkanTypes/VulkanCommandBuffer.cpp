#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandBuffer.h"

namespace Core
{
	void VulkanCommandBuffer::CreateCommandPool()
	{
		// Gets the queue families
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_PhysicalDevice, m_Surface);

		// Creates the pool infos
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		// Specifies the graphics queue for the command pool
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		// Creates the command pool
		VkResult result = vkCreateCommandPool(m_LogicalDevice, &poolInfo, nullptr, &m_CommandPool);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create command pool, Error Code: %d", result);
		}
	}

	void VulkanCommandBuffer::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		// Specifies the command pool
		allocInfo.commandPool = m_CommandPool;
		// Specifies if the command buffer is Primary or Secondary
		// Primary : Can be sent into a queue but can't be called by other command buffers
		// Secondary : Can be called from primary command buffers but can't be directly sent to a queue
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		// Specifies the number of command buffer to create
		allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

		// Creates the command buffer
		VkResult result = vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, m_CommandBuffers.data());

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to allocate command buffers, Error Code: %d", result);
		}
	}

	void VulkanCommandBuffer::RecordDrawCommandBuffer(VkCommandBuffer _CommandBuffer, uint32_t _ImageIndex)
	{
		// Command buffer begin recording infos
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		// Specifies the behavior of the command buffer (0 = None)
		beginInfo.flags = 0;
		// Only for secondary command buffers
		beginInfo.pInheritanceInfo = nullptr;

		// Start the command buffer
		VkResult result = vkBeginCommandBuffer(_CommandBuffer, &beginInfo);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to begin recording command buffer, Error Code: %d", result);
		}

		// Render pass infos
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		// Specifies render pass and attachments (FrameBuffer)
		renderPassInfo.renderPass = m_RenderPass;
		renderPassInfo.framebuffer = m_SwapChainFramebuffers[_ImageIndex];
		// Specifies render area
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChainExtent;

		// Creates the clear values
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { {0.2f, 0.2f, 1.f, 1.f} };	// Clear color
		clearValues[1].depthStencil = { 1.f, 0 };			// Clear depth

		// Specifies the clear values
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		// Starts the render pass
		// Last parameter: declares the origin of the commands here from a primary command buffer
		// VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFER -> for secondary command buffer
		vkCmdBeginRenderPass(_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Bind the pipeline (Second parameter specifies that this is a graphic pipeline and not a compute pipeline)
		vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

		// Defines the area where the image will be rendered
		VkViewport viewport{};
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = static_cast<float>(m_SwapChainExtent.width);
		viewport.height = static_cast<float>(m_SwapChainExtent.height);
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		vkCmdSetViewport(_CommandBuffer, 0, 1, &viewport);

		// Describes a limit where the image can be draw (similar to the viewport)
		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChainExtent;

		vkCmdSetScissor(_CommandBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
	}

	void VulkanCommandBuffer::DrawCommandBuffer(VkCommandBuffer _CommandBuffer)
	{
		VkBuffer vertexBuffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };

		// Binds the vertex buffer
		vkCmdBindVertexBuffers(_CommandBuffer, 0, 1, vertexBuffers, offsets);

		// Binds the index buffer
		vkCmdBindIndexBuffer(_CommandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

		// Binds descriptor sets (UBO)
		vkCmdBindDescriptorSets(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[m_CurrentFrame], 0, nullptr);

		// Draws the vertex buffer with indices
		vkCmdDrawIndexed(_CommandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
	}

	void VulkanCommandBuffer::EndDrawCommandBuffer(VkCommandBuffer _CommandBuffer)
	{
		// Finish the render pass
		vkCmdEndRenderPass(_CommandBuffer);

		// Stops recording commands
		VkResult result = vkEndCommandBuffer(_CommandBuffer);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to record command buffer, Error Code %d", result);
		}
	}

	VkCommandBuffer VulkanCommandBuffer::BeginSingleTimeCommands()
	{
		// Command buffer allocation infos
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;

		// Allocates the command buffer
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, &commandBuffer);

		// Begin info specifing VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT which means the command buffer will be used for a single command
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		// Start recording the command
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanCommandBuffer::EndSingleTimeCommands(VkCommandBuffer _CommandBuffer)
	{
		// Ends recording the command
		vkEndCommandBuffer(_CommandBuffer);

		// Submit infos
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_CommandBuffer;

		// Sumbit the queue
		vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

		// Wait for the actions to finish before destroying it
		vkQueueWaitIdle(m_GraphicsQueue);

		// Free and destroy the command buffer
		vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, 1, &_CommandBuffer);
	}
}