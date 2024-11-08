#include "RHI/VulkanRHI/VulkanRenderer.h"

#include <cstring>
#include <set>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Utils/Utils.h"

#include "Application.h"

namespace Core
{
	VulkanRenderer::~VulkanRenderer()
	{}

	const bool VulkanRenderer::Initialize(Window* _Window)
	{

		// Defines the window resize callback with swapchain recreation
		// Adds a custom pointer to our class to access it in any call back functions
		glfwSetWindowUserPointer(_Window->GetWindowPointer(), this);
		glfwSetFramebufferSizeCallback(_Window->GetWindowPointer(), FrameBufferResizeCallback);

		CreateSwapChain(_Window);
		CreateSwapChainImageViews();

		CreateRenderPass();
		CreateDescriptorSetLayout();
		CreateGraphicsPipeline();
		CreateCommandPool();
		CreateDepthRessources();
		CreateSwapChainFramebuffers();
		CreateTextureImage();
		CreateTextureImageView();
		CreateTextureSampler();
		LoadModel();
		CreateVertexBuffer();
		CreateIndexBuffer();
		CreateUniformBuffers();
		CreateDescriptorPool();
		CreateDescriptorSets();
		CreateCommandBuffers();
		CreateSyncObjects();

		return true;
	}

	const bool VulkanRenderer::Terminate()
	{
		// Destroys every objects created

		vkDeviceWaitIdle(m_LogicalDevice);

		CleanSwapChain();

		vkDestroySampler(m_LogicalDevice, m_TextureSampler, nullptr);
		vkDestroyImageView(m_LogicalDevice, m_TextureImageView, nullptr);

		vkDestroyImage(m_LogicalDevice, m_TextureImage, nullptr);
		vkFreeMemory(m_LogicalDevice, m_TextureImageMemory, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			vkDestroyBuffer(m_LogicalDevice, m_UniformBuffers[i], nullptr);
			vkFreeMemory(m_LogicalDevice, m_UniformBufferMemory[i], nullptr);
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			vkDestroySemaphore(m_LogicalDevice, m_ImageAvailableSemaphores[i], nullptr);

			vkDestroySemaphore(m_LogicalDevice, m_RenderFinishedSemaphores[i], nullptr);

			vkDestroyFence(m_LogicalDevice, m_InFlightFences[i], nullptr);
		}

		return true;
	}

	void VulkanRenderer::CreateSwapChainImageViews()
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); ++i)
		{
			// Creates an image view for each image of the swap chain
			m_SwapChainImageViews[i] = CreateImageView(m_SwapChainImages[i], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		return;
	}

	std::vector<char> VulkanRenderer::ReadSpirVShader(const std::filesystem::path& _FilePath)
	{
		// Cancels the reading if the file is not a SpirV file 
		if (_FilePath.extension() != ".spv")
		{
			DEBUG_ERROR("Wrong shader file extension");
			std::vector<char> err(0);
			return err;
		}

		std::ifstream file(_FilePath, std::ios::ate | std::ios::binary);

		// Cancels the reading if the file failed to open
		if (!file.is_open())
		{
			DEBUG_ERROR("Failed to open shader file");
			std::vector<char> err(0);
			return err;
		}

		// Reads the file
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		// Closes the file
		file.close();

		// Returns the buffer filled by the file
		return buffer;
	}

	VkShaderModule VulkanRenderer::CreateShaderModule(const std::vector<uint32_t>& _ShaderSourceCode)
	{
		// Create info of the shader
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		// Specifies the size and the code of the source code
		createInfo.codeSize = _ShaderSourceCode.size() * sizeof(uint32_t);
		createInfo.pCode = _ShaderSourceCode.data();

		// Creates the shader module
		VkShaderModule shaderModule;
		VkResult result = vkCreateShaderModule(m_LogicalDevice, &createInfo, nullptr, &shaderModule);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create shader module, Error Code: %d", result);
		}

		return shaderModule;
	}

	std::vector<uint32_t> VulkanRenderer::CompileShader(std::filesystem::path _ShaderPath, ShaderType _ShaderType)
	{
		// Causes memory leaks, thx
		shaderc::Compiler compiler;

		// TODO: Move shader reading in the resource loader
		// Reads the shader
		std::ifstream shaderFile(_ShaderPath);
		std::string line = "";
		std::string shaderCode = "";

		if (!shaderFile.is_open())
		{
			DEBUG_ERROR("Failed to open shader");
		}

		while (std::getline(shaderFile, line))
			shaderCode += line + "\n";

		shaderFile.close();

		// Fill a struct of informations about the shader
		CompilationInfos infos{};
		infos.fileName = _ShaderPath.filename().string().c_str();

		switch (_ShaderType)
		{
		case VERTEX: default:
			infos.shaderKind = shaderc_vertex_shader;
			break;
		case FRAGMENT:
			infos.shaderKind = shaderc_fragment_shader;
			break;
		}

		//infos.options.SetOptimizationLevel(shaderc_optimization_level_performance);
		//infos.options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		infos.sourceCode = &shaderCode;

		// First step - Preprocessing GLSL
		shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(*infos.sourceCode, infos.shaderKind, infos.fileName, infos.options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to preprocessed shader, %s", result.GetErrorMessage())
		}

		// Copy the precompiled code in a buffer
		const char* src = result.cbegin();
		size_t newSize = result.cend() - src;
		infos.sourceCode->resize(newSize);
		memcpy(infos.sourceCode->data(), src, newSize);

		// Second step - SPIR-V Assembly compilation
		shaderc::AssemblyCompilationResult sResult = compiler.CompileGlslToSpvAssembly(*infos.sourceCode, infos.shaderKind, infos.fileName, infos.options);

		if (sResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to compiled in assembly shader, %s", sResult.GetErrorMessage())
		}

		// Copy the SPRIV Assembly code in a buffer
		src = sResult.cbegin();
		newSize = sResult.cend() - src;
		infos.sourceCode->resize(newSize);
		memcpy(infos.sourceCode->data(), src, newSize);

		// Third step - SPIR-V Binary compilation
		shaderc::SpvCompilationResult tResult = compiler.AssembleToSpv(infos.sourceCode->data(), infos.sourceCode->size(), infos.options);

		if (tResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			DEBUG_ERROR("Failed to compiled in binary shader, %s", tResult.GetErrorMessage())
		}

		// Copy the SPIRV Binary code in the output buffer
		const uint32_t* srcBinary = tResult.cbegin();
		size_t wordCount = tResult.cend() - srcBinary;
		std::vector<uint32_t> output(wordCount);
		memcpy(output.data(), srcBinary, wordCount * sizeof(uint32_t));

		return output;
	}

	void VulkanRenderer::CreateSwapChainFramebuffers()
	{
		m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

		// Loop going through every image view of the swap chain
		for (size_t i = 0; i < m_SwapChainImageViews.size(); ++i)
		{
			// Specifies the attachments for each frame buffer
			std::array<VkImageView, 2> attachments = {
				m_SwapChainImageViews[i],			// ColorBuffer
				m_DepthImageView					// Depth Buffer
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			// Specifies the renderpass compatible with the framebuffer
			framebufferInfo.renderPass = m_RenderPass;
			// Specifies the attachments
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = m_SwapChainExtent.width;
			framebufferInfo.height = m_SwapChainExtent.height;
			// Layer number - More than 1 for 3D stereoscopic app
			framebufferInfo.layers = 1;

			// Creates the frambuffer
			VkResult result = vkCreateFramebuffer(m_LogicalDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create Framebuffer, Error Code: %d", result);
			}
		}
	}

	void VulkanRenderer::DrawFrame(Window* _Window)
	{
		// Will wait the end of the rendering of the current frame (if rendering) and reset the fence
		// fourth parameter specifies that we want to wait for all the fences to finish (here we have only one fence)
		// last parameter specifies the delay before canceling (UINT64_MAX = cancel delay can wait indefinitely)
		vkWaitForFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]);

		uint32_t imageIndex;
		// Acquires an image from the swap chain
		// third parameter is a delay before canceling if no image is available -> here it's at UINT64_MAX which disable the delay
		// fourth and fifith parameter is the semaphore or fence that will be trigered when an image is available
		// the last parameter is the index of the image in the swap chain
		VkResult result = vkAcquireNextImageKHR(m_LogicalDevice, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain(_Window);
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			DEBUG_ERROR("Failed to acquire swap chain image, Error Code: %d", result);
		}

		Math::Vector3 pos = Math::Vector3::zero;
		Math::Vector3 rot = Math::Vector3::zero;
		Math::Vector3 scale = Math::Vector3::one;

		Math::Matrix4 model[] = { Math::Matrix4::TRS(pos, rot, scale),  Math::Matrix4::TRS(pos, rot, scale) };

		vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);
		RecordDrawCommandBuffer(m_CommandBuffers[m_CurrentFrame], imageIndex);

		for (size_t i = 0; i < 2; ++i)
		{
			// Updates the uniform buffer
			UpdateUniformBuffer(m_CurrentFrame, model[i]);

			DrawCommandBuffer(m_CommandBuffers[m_CurrentFrame]);
		}

		EndDrawCommandBuffer(m_CommandBuffers[m_CurrentFrame]);

		// Submit informations
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		// Here we reference the waiting semaphore and waiting stage
		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		// Specifies to vulkan to wait for m_ImageAvailableSemaphores at VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT to start rendering
		// We do not want to write the colors without an image so when an image is available vulkan can write in the image
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		// Specifies the command buffer(s) that will be executed
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame];

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };

		// Specifies the semaphores that will indicates if the frame finished rendering
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		// Sumbit the command buffer(s) for execution to the graphic queue
		// The last parameter is the fence that will signal when the frame finished rendering
		result = vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to submit draw command buffer, Error Code %d", result);
		}

		// Presentation informations
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		
		// Specifies the semaphore to wait for in order to present an image
		// Here we do not want to present an image it is not finished rendering
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain };

		// Reference our swap chain that will present the images
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		// Present the image with the presentation queue
		result = vkQueuePresentKHR(m_PresentQueue, &presentInfo);

		// Checks if the swap chain is no more compatible or inadequate for presentation or if the window has been resized
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized)
		{
			// Recreates the swap chain
			m_FramebufferResized = false;
			RecreateSwapChain(_Window);
		}
		else if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to present swap chain image, Error Code: %d", result);
		}

		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::CreateSyncObjects()
	{
		m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		// Semaphore create info - for GPU/ GPU synchronization
		VkSemaphoreCreateInfo smaphoreInfo{};
		smaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		// Fence create info - for CPU / GPU synchronization
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// Specifies that the fence is created as signaled to avoid an infinite wait at the start of the draw
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		// Loop for all MAX_FRAMES_IIN_FLIGHT
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			// Creates Image available semaphores
			VkResult result = vkCreateSemaphore(m_LogicalDevice, &smaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create semaphore, Error Code: %d", result)
			}

			// Creates Render Finish semaphores
			result = vkCreateSemaphore(m_LogicalDevice, &smaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create semaphore, Error Code: %d", result)
			}

			// Creates In Flight fences
			result = vkCreateFence(m_LogicalDevice, &fenceInfo, nullptr, &m_InFlightFences[i]);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create fence, Error Code: %d", result)
			}
		}
	}

	void VulkanRenderer::FrameBufferResizeCallback(GLFWwindow* _Window, int _Width, int _Height)
	{
		// Gets the custom pointer and cast it to our class
		VulkanRenderer* vkWrapper = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(_Window));

		vkWrapper->m_FramebufferResized = true;
	}

	void VulkanRenderer::CreateUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformMVP);

		m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		m_UniformBufferMemory.resize(MAX_FRAMES_IN_FLIGHT);
		m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

		// Creates a single uniform buffer for each frames
		// We do not use staging buffer because the data will be updated every frame
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			// Creates a Uniform buffer
			CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBufferMemory[i]);
			// Maps the Memory
			vkMapMemory(m_LogicalDevice, m_UniformBufferMemory[i], 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
		}
	}

	void VulkanRenderer::UpdateUniformBuffer(uint32_t _CurrentImage, Math::Matrix4 _ModelMatrix)
	{
		// Creates a MVP
		UniformMVP mvp{};

		// Updates the MVP
		mvp.model = _ModelMatrix.Transpose();
		mvp.view = Core::Application::appCamera.viewMatrix.Transpose();
		mvp.projection = Core::Application::appCamera.projectionMatrix.Transpose();

		// Copies the data into the buffer
		memcpy(m_UniformBuffersMapped[_CurrentImage], &mvp, sizeof(mvp));
	}

	VkImageView VulkanRenderer::CreateImageView(VkImage _Image, VkFormat _Format, VkImageAspectFlags _AspectFlags)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _Image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = _Format;
		viewInfo.subresourceRange.aspectMask = _AspectFlags;
		// Mipmap levels
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		// baseArrayLayer - layerCount is the number of layers an image possess - only usefull for 3D stereoscopic applications
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		// Creates the image view
		VkImageView imageView;
		VkResult result = vkCreateImageView(m_LogicalDevice, &viewInfo, nullptr, &imageView);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture image view, Error Code: %d", result);
			return nullptr;
		}

		return imageView;
	}

	void VulkanRenderer::CreateTextureImage()
	{
		// Loads a texture with STB IMAGE
		int textWidth, textHeight, textChannels;
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* texture = stbi_load(TEXTURE_PATH.c_str(), &textWidth, &textHeight, &textChannels, STBI_rgb_alpha);

		VkDeviceSize imageSize = textWidth * textHeight * 4;

		if (!texture)
		{
			DEBUG_ERROR("Failed to load texture");
			return;
		}

		// Creates transfer buffers
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Creates buffer that can transfer memory from CPU to GPU
		CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		// Map the data into the buffer
		void* data;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, texture, static_cast<size_t>(imageSize));
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Free the texture on the CPU
		stbi_image_free(texture);

		// Creates the image object
		CreateImage(static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);

		// Transition the image layout a first time
		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy the texture in the good buffer
		CopyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(textWidth), static_cast<uint32_t>(textHeight));

		// Transition the image layout a second time
		TransitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Destroys the transfer buffer
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
	}

	void VulkanRenderer::CreateImage(uint32_t _Width, uint32_t _Height, VkFormat _Format, VkImageTiling _Tiling, VkImageUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkImage& _Image, VkDeviceMemory& _ImageMemory)
	{
		// Omage infos
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		// Type of texture
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = _Width;
		imageInfo.extent.height = _Height;
		// Depth is 1 because it's a 2D texture
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = _Format;
		// How pixels are organized in the memory
		imageInfo.tiling = _Tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = _Usage;
		// For multi sampling
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		// Defines if the image can be used by multiple queues
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		// Creates the image
		VkResult result = vkCreateImage(m_LogicalDevice, &imageInfo, nullptr, &_Image);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create image, Error Code: %d", result);
		}

		// Gets the memory requirements for the texture
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_LogicalDevice, _Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		// Allocates the memory
		result = vkAllocateMemory(m_LogicalDevice, &allocInfo, nullptr, &_ImageMemory);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Filed to allocate image memory, Error Code: %d", result);
		}

		// Bind the memory with our image
		vkBindImageMemory(m_LogicalDevice, _Image, _ImageMemory, 0);
	}

	void VulkanRenderer::TransitionImageLayout(VkImage _Image, VkFormat _Format, VkImageLayout _OldLayout, VkImageLayout _NewLayout)
	{
		// Starts recording command buffer
 		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		// Creates a memory barrier
		// Is here to garanty that we will not modify the memory while the GPU is using it or already writing in it
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;\
		// Specifies the old layout and the new layout
		barrier.oldLayout = _OldLayout;
		barrier.newLayout = _NewLayout;
		// Used to change queue families
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		// Base data of the image
		barrier.image = _Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		// Default values
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = 0;

		// Creates src and dst stages that we have to defined
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		// For depth buffer
		if (_NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(_Format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}

		// Handles some transitions
		if (_OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (_OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (_OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			// Also for depth buffer
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			DEBUG_ERROR("Unsupported layout transition !");
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		// Ends recording the command buffer
		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanRenderer::CopyBufferToImage(VkBuffer _Buffer, VkImage _Image, uint32_t _Width, uint32_t _Height)
	{
		// Records command buffer
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		// Creates the region to copy
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { _Width, _Height, 1 };

		// Copies the buffer to the image
		vkCmdCopyBufferToImage(commandBuffer, _Buffer, _Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		// Ends recording the command buffer
		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanRenderer::CreateTextureImageView()
	{
		// Uses the create image view function
		m_TextureImageView = CreateImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void VulkanRenderer::CreateTextureSampler()
	{
		// Sampler infos
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		// Same as OpenGL
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16.0f;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		// VK_FALSE = coords are between 0 and width or height and VK_TRUE coords are between 0 and 1
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		// Mip maps infos
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		// Creates the sampler
		VkResult result = vkCreateSampler(m_LogicalDevice, &samplerInfo, nullptr, &m_TextureSampler);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create texture sampler, Error Code: %d", result);
			return;
		}
	}

	void VulkanRenderer::CreateDepthRessources()
	{
		// Check for the best depth format available
		VkFormat depthFormat = FindDepthFormat();

		// Creates an image of the size of our rendering viewport (swap chain) - Creates also the image view to have access to the depth buffer
		CreateImage(m_SwapChainExtent.width, m_SwapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_DepthImage, m_DepthImageMemory);
		m_DepthImageView = CreateImageView(m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

		// Transition the layout
		TransitionImageLayout(m_DepthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	VkFormat VulkanRenderer::FindSupportedFormat(const std::vector<VkFormat>& _Candidates, VkImageTiling _Tiling, VkFormatFeatureFlags _Features)
	{
		// Loop through all candidates
		for (VkFormat format : _Candidates)
		{
			// Gets the formats properties in our device
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);

			// Checks the type of _Tiling and if the features required are supported
			if (_Tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & _Features) == _Features)
			{
				return format;
			}
			else if (_Tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & _Features) == _Features)
			{
				return format;
			}
		}

		DEBUG_ERROR("Failed to find supported format!");
	}

	VkFormat VulkanRenderer::FindDepthFormat()
	{
		// Checks a suported format for depth texture
		return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	bool VulkanRenderer::HasStencilComponent(VkFormat _Format)
	{
		return _Format == VK_FORMAT_D32_SFLOAT_S8_UINT || _Format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
}