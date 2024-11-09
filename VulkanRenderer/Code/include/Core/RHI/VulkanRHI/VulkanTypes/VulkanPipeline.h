#pragma once

#include "RHI/RHITypes/IPipeline.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class IDevice;

	class VulkanPipeline : public IPipeline
	{
	private:
		VkPipeline m_GraphicsPipeline;
		VkRenderPass m_RenderPass;
		VkPipelineLayout m_PipelineLayout;

	public:
		~VulkanPipeline() override;

		/// <summary>
		/// Creates a simple graphics pipeline to draw on screen
		/// </summary>
		RHI_RESULT CreatePipeline(IDevice* _Device, IShader* _VertexShader, IShader* _FragmentShader) override;
		RHI_RESULT DestroyPipeline(IDevice* _Device) override;

		/// <summary>
		/// Creates a render pass object that will descibes the attachments of a framebuffer
		/// </summary>
		void CreateRenderPass(IDevice* _Device);

		inline VkPipeline GetPipeline() const { return m_GraphicsPipeline; }
		inline VkRenderPass GetRenderPass() const { return m_RenderPass; }
		inline VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }

		inline VulkanPipeline* CastToVulkan() override { return this; }
	};
}