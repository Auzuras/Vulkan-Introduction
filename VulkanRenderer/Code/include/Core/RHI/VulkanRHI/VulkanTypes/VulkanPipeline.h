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

		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;

	public:
		~VulkanPipeline() override;

		/// <summary>
		/// Creates a simple graphics pipeline to draw on screen
		/// </summary>
		RHI_RESULT CreatePipeline(IDevice* _Device, ISwapChain* _Swapchain, std::vector<PipelineShaderInfos> _ShadersInfos) override;
		RHI_RESULT DestroyPipeline(IDevice* _Device) override;

		/// <summary>
		/// Creates all the descriptor set layout to describe the UBO or global variables
		/// </summary>
		const RHI_RESULT CreateDescriptorSetLayout(VulkanDevice* _Device);

		/// <summary>
		/// Creates a render pass object that will descibes the attachments of a framebuffer
		/// </summary>
		void CreateRenderPass(VulkanDevice* _Device, VulkanSwapChain* _Swapchain);

		inline VkPipeline GetPipeline() const { return m_GraphicsPipeline; }
		inline VkRenderPass GetRenderPass() const { return m_RenderPass; }
		inline VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }

		inline VulkanPipeline* CastToVulkan() override { return this; }
	};
}