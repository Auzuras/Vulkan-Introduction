#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandBuffer.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandAllocator.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanMesh.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanPipeline.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptor.h"

namespace Core
{
	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer _CommandBuffer)
		:m_CommandBuffer(_CommandBuffer)
	{}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}

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

	void VulkanCommandBuffer::StartRenderPass(IPipeline* _Pipeline, ISwapChain* _Swapchain, unsigned int _ImageIndex, Math::Vector4 _ClearColor) const
	{
		VulkanPipeline pipeline = *_Pipeline->CastToVulkan();
		VulkanSwapChain swapchain = *_Swapchain->CastToVulkan();

		// Render pass infos
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		// Specifies render pass and attachments (FrameBuffer)
		renderPassInfo.renderPass = pipeline.GetRenderPass();
		renderPassInfo.framebuffer = swapchain.GetSwapchainFramebuffers(_ImageIndex).GetFrameBuffer();
		// Specifies render area
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapchain.GetSwapchainExtent();

		// Creates the clear values
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { {_ClearColor[0], _ClearColor[1], _ClearColor[2], _ClearColor[3]} };	// Clear color
		clearValues[1].depthStencil = { 1.f, 0 };														// Clear depth

		// Specifies the clear values
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		// Starts the render pass
		// Last parameter: declares the origin of the commands here from a primary command buffer
		// VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFER -> for secondary command buffer
		vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanCommandBuffer::BindPipeline(IPipeline* _Pipeline) const
	{
		VulkanPipeline pipeline = *_Pipeline->CastToVulkan();
		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipeline());
	}

	void VulkanCommandBuffer::SetViewport(Math::Vector2 _Position, ISwapChain* _Swapchain, float _MinDepth, float _MaxDepth) const
	{
		VulkanSwapChain swapchain = *_Swapchain->CastToVulkan();

		VkViewport viewport{};
		viewport.x = _Position[0];
		viewport.y = _Position[1];
		viewport.width = static_cast<float>(swapchain.GetSwapchainExtent().width);
		viewport.height = static_cast<float>(swapchain.GetSwapchainExtent().height);
		viewport.minDepth = _MinDepth;
		viewport.maxDepth = _MaxDepth;

		vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);
	}

	void VulkanCommandBuffer::SetScissor(Math::Vector2 _Offset, ISwapChain* _Swapchain) const
	{
		VulkanSwapChain swapchain = *_Swapchain->CastToVulkan();

		// Describes a limit where the image can be draw (similar to the viewport)
		VkRect2D scissor{};
		scissor.offset = { static_cast<int>(_Offset[0]), static_cast<int>(_Offset[1]) };
		scissor.extent = swapchain.GetSwapchainExtent();

		vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);
	}

	void VulkanCommandBuffer::BindDescriptorSet(IPipeline* _Pipeline, IDescriptor* _DescriptorSet, unsigned int _SetBiding) const
	{
		VulkanPipeline layout = *_Pipeline->CastToVulkan();
		VulkanDescriptor descriptor = *_DescriptorSet->CastToVulkan();

		vkCmdBindDescriptorSets(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout.GetPipelineLayout(), _SetBiding, 1, &descriptor.m_DescriptorSets, 0, nullptr);
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
		VulkanMesh mesh = *_Mesh->CastToVulkan();

		// Draws the vertex buffer with indices
		vkCmdDrawIndexed(m_CommandBuffer, static_cast<uint32_t>(mesh.GetIndexNumber()), 1, 0, 0, 0);
	}

	void VulkanCommandBuffer::EndRenderPass() const
	{
		// Finish the render pass
		vkCmdEndRenderPass(m_CommandBuffer);
	}
}