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

		/// <summary>
		/// Creates a simple graphics pipeline to draw on screen
		/// </summary>
		RHI_RESULT CreatePipeline(IDevice* _Device);
		RHI_RESULT DestroyPipeline();

		/// <summary>
		/// Creates a render pass object that will descibes the attachments of a framebuffer
		/// </summary>
		void CreateRenderPass();
	};
}