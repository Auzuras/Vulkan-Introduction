#include "RHI/VulkanRHI/VulkanTypes/VulkanPipeline.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

namespace Core
{
	VulkanPipeline::~VulkanPipeline()
	{}

	RHI_RESULT VulkanPipeline::CreatePipeline(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Creates vertex shader infos
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		// Stage of the pipeline (Which momement the shader will be called)
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		// Shader module
		//vertShaderStageInfo.module = vertShaderModule;
		// Start function of the shader
		vertShaderStageInfo.pName = "main";

		// Creates fragment shader infos
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		// Stage of the pipeline (Which momement the shader will be called)
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		// Shader module
		//fragShaderStageInfo.module = fragShaderModule;
		// Start function of the shader
		fragShaderStageInfo.pName = "main";

		// List of all the shaders of the pipeline
		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		// Gets the vertex binding and attribute descriptions
		VkVertexInputBindingDescription bindingDescription = Core::Vertex::GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = Core::Vertex::GetAttributeDescriptions();

		// Describes how the vertex will be inputted in the first shader 
		// (Correspond to the layout(binding=0) position etc in the shader)
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		// How much vertices we are describing
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		// The number of attribute per vertex (Position, Color, Normal ...)
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		// The binding description ()
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		// The attributes description (Binding, size, offset, type)
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		// Describes the geometry
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		// Describes how the geometry should be drawn
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		// Can cancel some links between vertices
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Reference all dynamics states
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		// Dynamic states infos
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		// Rasterizer informations
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		// Fargments above a under the near and far plane are clamped
		rasterizer.depthClampEnable = VK_FALSE;
		// Desactivates rasterizer and steps after
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		// Defines how the polygons are drawn (Fill, poins, lines)
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.f;
		// Defines culling
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		// Modifies the depth acording to a fragment (usefull for shadow mapping)
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.f;
		rasterizer.depthBiasClamp = 0.f;
		rasterizer.depthBiasSlopeFactor = 0.f;

		// Multisampling infos
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		// Activates or not the multisampling
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		// Activate depth test
		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		// Comparison test to eleminate or keep frag
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;

		// Color blending infos (The color given by the fragshader is blend with the other color at the same fragment)
		// This structure describes how it should be blend
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// Specifies how the color blend with many frame buffers
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.f;
		colorBlending.blendConstants[1] = 0.f;
		colorBlending.blendConstants[2] = 0.f;
		colorBlending.blendConstants[3] = 0.f;

		// Pipeline layout infos
		// Describes all uniform buffers present in our pipeline
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		// References the Descriptors set layout (UBO) or global variables
		pipelineLayoutInfo.setLayoutCount = 1;
		//pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		// Creates the pipeline layout
		VkResult result = vkCreatePipelineLayout(device.GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create pipeline layout, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		// References all the information created
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		// Describes the number of stages (shaders in our pipeline)
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		// Specifies all the information previously created
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		// Describes the layout of the pipeline
		pipelineInfo.layout = m_PipelineLayout;
		// Describes the render pass
		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;
		// Reference a new pipeline to create from this one (not necessary here)
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		// Creates the pipeline
		result = vkCreateGraphicsPipelines(device.GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create graphics pipeline, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		// Destroys the shader module because they are loaded on the GPU
		//vkDestroyShaderModule(device.GetLogicalDevice(), fragShaderModule, nullptr);
		//vkDestroyShaderModule(device.GetLogicalDevice(), vertShaderModule, nullptr);
	}

	RHI_RESULT VulkanPipeline::DestroyPipeline(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroyPipeline(device.GetLogicalDevice(), m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device.GetLogicalDevice(), m_PipelineLayout, nullptr);
		vkDestroyRenderPass(device.GetLogicalDevice(), m_RenderPass, nullptr);

		return RHI_SUCCESS;
	}

	void VulkanPipeline::CreateRenderPass(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Describes the color buffer attachment
		VkAttachmentDescription colorAttachment{};
		// Same format as the swap chain
		//colorAttachment.format = m_SwapChainImageFormat;
		// Samples (usefull for multisampling)
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		// LoadOp and StoreOp describes how we interact with the data of the buffer before loading and after rendering
		// VK_ATTACHMENT_LOAD_OP_CLEAR will clear all the information with a constant
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		// The rendered frame will be stored
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		// For the stencil buffer we juste dont care about what will be load and unload
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		// 
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Describe the color attachment
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Describes the depth buffer attachment - Check above for more infos (Similar to color attachment)
		VkAttachmentDescription depthAttachment{};
		//depthAttachment.format = FindDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Describe the depth attachment
		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Creates the subpass with color and depth attachment
		VkSubpassDescription subpass{};
		// Defines the subpass as a graphic one (can be a compute one)
		// Binds the depth attachment to the subpass
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		// Stocks the attachments in a list
		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		// Create info of the render pass
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		// Nbr of attachments
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		// Data of attachments
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		// Creates the render pass
		VkResult result = vkCreateRenderPass(device.GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create render pass, Error Code: %d", result);
		}
	}
}