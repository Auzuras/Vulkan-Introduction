#include "RHI/VulkanRHI/VulkanRenderer.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanShader.h"

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
		//glfwSetWindowUserPointer(_Window->GetWindowPointer(), this);
		//glfwSetFramebufferSizeCallback(_Window->GetWindowPointer(), FrameBufferResizeCallback);

		return true;
	}

	const bool VulkanRenderer::Terminate()
	{
		// Destroys every objects created

		//vkDeviceWaitIdle(m_LogicalDevice);


		return true;
	}

	IDevice* VulkanRenderer::CreateDevice()
	{
		VulkanDevice* vkDevice = new VulkanDevice;
		/*if (!vkDevice->Initialize())
			return nullptr;*/

		return vkDevice;

	}

	Resources::IShader* VulkanRenderer::CompileShader(std::string _ShaderSourceCode, ShaderType _ShaderType)
	{
		VulkanShader* shader = new VulkanShader;

		// Causes memory leaks, thx
		shaderc::Compiler compiler;

		// Fill a struct of informations about the shader
		CompilationInfos infos{};
		infos.fileName;// = _ShaderPath.filename().string().c_str();

		switch (_ShaderType)
		{
		case VERTEX: default:
			infos.shaderKind = shaderc_vertex_shader;
			break;
		case FRAGMENT:
			infos.shaderKind = shaderc_fragment_shader;
			break;
		case GEOMETRY:
			infos.shaderKind = shaderc_geometry_shader;
			break;
		}

		// TODO: Checks the optimizations
		//infos.options.SetOptimizationLevel(shaderc_optimization_level_performance);
		//infos.options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

		infos.sourceCode = &_ShaderSourceCode;

		if (!shader->PreprocessShader(compiler, infos))
			return nullptr;

		if (!shader->SpirVAssemblyCompilation(compiler, infos))
			return nullptr;

		std::vector<uint32_t> compiledShader = shader->SpirVBinaryCompilation(compiler, infos);

		if (compiledShader.empty())
			return nullptr;

		// Create shadermodule

		return shader;
	}

	//VkShaderModule VulkanRenderer::CreateShaderModule(const std::vector<uint32_t>& _ShaderSourceCode)
//{
//	// Create info of the shader
//	VkShaderModuleCreateInfo createInfo{};
//	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//	// Specifies the size and the code of the source code
//	createInfo.codeSize = _ShaderSourceCode.size() * sizeof(uint32_t);
//	createInfo.pCode = _ShaderSourceCode.data();

//	// Creates the shader module
//	VkShaderModule shaderModule;
//	VkResult result = vkCreateShaderModule(m_LogicalDevice, &createInfo, nullptr, &shaderModule);

//	if (result != VK_SUCCESS)
//	{
//		DEBUG_ERROR("Failed to create shader module, Error Code: %d", result);
//	}

//	return shaderModule;
//}

	//void VulkanRenderer::DrawFrame(Window* _Window)
	//{
	//	// Will wait the end of the rendering of the current frame (if rendering) and reset the fence
	//	// fourth parameter specifies that we want to wait for all the fences to finish (here we have only one fence)
	//	// last parameter specifies the delay before canceling (UINT64_MAX = cancel delay can wait indefinitely)
	//	vkWaitForFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);
	//	vkResetFences(m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]);

	//	uint32_t imageIndex;
	//	// Acquires an image from the swap chain
	//	// third parameter is a delay before canceling if no image is available -> here it's at UINT64_MAX which disable the delay
	//	// fourth and fifith parameter is the semaphore or fence that will be trigered when an image is available
	//	// the last parameter is the index of the image in the swap chain
	//	VkResult result = vkAcquireNextImageKHR(m_LogicalDevice, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

	//	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	//	{
	//		RecreateSwapChain(_Window);
	//		return;
	//	}
	//	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	//	{
	//		DEBUG_ERROR("Failed to acquire swap chain image, Error Code: %d", result);
	//	}

	//	Math::Vector3 pos = Math::Vector3::zero;
	//	Math::Vector3 rot = Math::Vector3::zero;
	//	Math::Vector3 scale = Math::Vector3::one;

	//	Math::Matrix4 model[] = { Math::Matrix4::TRS(pos, rot, scale),  Math::Matrix4::TRS(pos, rot, scale) };

	//	vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);
	//	RecordDrawCommandBuffer(m_CommandBuffers[m_CurrentFrame], imageIndex);

	//	for (size_t i = 0; i < 2; ++i)
	//	{
	//		// Updates the uniform buffer
	//		UpdateUniformBuffer(m_CurrentFrame, model[i]);

	//		DrawCommandBuffer(m_CommandBuffers[m_CurrentFrame]);
	//	}

	//	EndDrawCommandBuffer(m_CommandBuffers[m_CurrentFrame]);

	//	// Submit informations
	//	VkSubmitInfo submitInfo{};
	//	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	//	// Here we reference the waiting semaphore and waiting stage
	//	VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
	//	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	//	// Specifies to vulkan to wait for m_ImageAvailableSemaphores at VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT to start rendering
	//	// We do not want to write the colors without an image so when an image is available vulkan can write in the image
	//	submitInfo.waitSemaphoreCount = 1;
	//	submitInfo.pWaitSemaphores = waitSemaphores;
	//	submitInfo.pWaitDstStageMask = waitStages;

	//	// Specifies the command buffer(s) that will be executed
	//	submitInfo.commandBufferCount = 1;
	//	submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame];

	//	VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };

	//	// Specifies the semaphores that will indicates if the frame finished rendering
	//	submitInfo.signalSemaphoreCount = 1;
	//	submitInfo.pSignalSemaphores = signalSemaphores;

	//	// Sumbit the command buffer(s) for execution to the graphic queue
	//	// The last parameter is the fence that will signal when the frame finished rendering
	//	result = vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]);

	//	if (result != VK_SUCCESS)
	//	{
	//		DEBUG_ERROR("Failed to submit draw command buffer, Error Code %d", result);
	//	}

	//	// Presentation informations
	//	VkPresentInfoKHR presentInfo{};
	//	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//	
	//	// Specifies the semaphore to wait for in order to present an image
	//	// Here we do not want to present an image it is not finished rendering
	//	presentInfo.waitSemaphoreCount = 1;
	//	presentInfo.pWaitSemaphores = signalSemaphores;

	//	VkSwapchainKHR swapChains[] = { m_SwapChain };

	//	// Reference our swap chain that will present the images
	//	presentInfo.swapchainCount = 1;
	//	presentInfo.pSwapchains = swapChains;
	//	presentInfo.pImageIndices = &imageIndex;
	//	presentInfo.pResults = nullptr;

	//	// Present the image with the presentation queue
	//	result = vkQueuePresentKHR(m_PresentQueue, &presentInfo);

	//	// Checks if the swap chain is no more compatible or inadequate for presentation or if the window has been resized
	//	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized)
	//	{
	//		// Recreates the swap chain
	//		m_FramebufferResized = false;
	//		RecreateSwapChain(_Window);
	//	}
	//	else if (result != VK_SUCCESS)
	//	{
	//		DEBUG_ERROR("Failed to present swap chain image, Error Code: %d", result);
	//	}

	//	m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	//}

	//void VulkanRenderer::FrameBufferResizeCallback(GLFWwindow* _Window, int _Width, int _Height)
	//{
	//	// Gets the custom pointer and cast it to our class
	//	VulkanRenderer* vkWrapper = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(_Window));

	//	vkWrapper->m_FramebufferResized = true;
	//}
}