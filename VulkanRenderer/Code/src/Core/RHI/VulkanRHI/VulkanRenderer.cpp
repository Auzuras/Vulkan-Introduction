#include "RHI/VulkanRHI/VulkanRenderer.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanPipeline.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandAllocator.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanShader.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanMesh.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanTexture.h"

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

	IDevice* VulkanRenderer::InstanciateDevice(Window* _Window)
	{
		VulkanDevice* vkDevice = new VulkanDevice;

		if (!vkDevice->Initialize(_Window))
			return nullptr;

		return vkDevice;
	}

	ISwapChain* VulkanRenderer::InstantiateSwapChain(Window* _Window, IDevice* _Device)
	{
		VulkanSwapChain* vkSwapChain = new VulkanSwapChain;

		if (!vkSwapChain->CreateSwapChain(_Window, _Device))
			return nullptr;

		return vkSwapChain;
	}

	IPipeline* VulkanRenderer::InstantiatePipeline(IDevice* _Device, ISwapChain* _Swapchain, std::vector<PipelineShaderInfos> _ShadersInfos)
	{
		VulkanPipeline* vkPipeline = new VulkanPipeline;

		if (!vkPipeline->CreatePipeline(_Device, _Swapchain, _ShadersInfos))
			return nullptr;

		return vkPipeline;
	}

	ICommandAllocator* VulkanRenderer::InstantiateCommandAllocator(IDevice* _Device)
	{
		VulkanCommandAllocator* vkCmdAllocator = new VulkanCommandAllocator;

		if (!vkCmdAllocator->CreateCommandAllocator(_Device))
			return nullptr;

		return vkCmdAllocator;
	}

	IShader* VulkanRenderer::CreateShader()
	{
		VulkanShader* vkShader = new VulkanShader;
		return vkShader;
	}
	
	IMesh* VulkanRenderer::CreateMesh()
	{
		VulkanMesh* vkMesh = new VulkanMesh;
		return vkMesh;
	}

	ITexture* VulkanRenderer::CreateTexture()
	{
		VulkanTexture* vkTexture = new VulkanTexture;
		return vkTexture;
	}
}