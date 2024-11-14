#pragma once

#include "RHI/RHITypes/ICommandBuffer.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanCommandBuffer : public ICommandBuffer
	{
	private:
		VkCommandBuffer m_CommandBuffer;

	public:

		VulkanCommandBuffer() = default;
		VulkanCommandBuffer(VkCommandBuffer _CommandBuffer);

		/// <summary>
		/// Creates a temporary command buffer for a single command
		/// </summary>
		/// <returns></returns>
		static VkCommandBuffer BeginSingleTimeCommands(VulkanDevice* _Device, VulkanCommandAllocator* _CommandAllocator);

		/// <summary>
		/// Ends and destroy a temporary command buffer for a single command
		/// </summary>
		/// <param name="_CommandBuffer">: Command buffer to destroy </param>
		static void EndSingleTimeCommands(VulkanDevice* _Device, VulkanCommandAllocator* _CommandAllocator, VkCommandBuffer _CommandBuffer);

		inline VkCommandBuffer& GetCommandBuffer() { return m_CommandBuffer;  }

		void ResetCommandBuffer() override;

		const RHI_RESULT StartRecordingCommandBuffer() override;
		const RHI_RESULT StopRecordingCommandBuffer() const override;

		void BindVertexBuffer(IMesh* _Mesh) const override;
		void BindIndexBuffer(IMesh* _Mesh) const override;
		void DrawIndexed(IMesh* _Mesh) const override;
		void EndRenderPass() const override;
	};
}