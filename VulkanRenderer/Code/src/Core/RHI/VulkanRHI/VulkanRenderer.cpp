#include "RHI/VulkanRHI/VulkanRenderer.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanSwapChain.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanPipeline.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandAllocator.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanShader.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanMesh.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanTexture.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanCommandBuffer.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptorAllocator.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanSemaphore.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanFence.h"

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

	IDescriptorAllocator* VulkanRenderer::InstantiateDescriptorAllocator(IDevice* _Device, ISwapChain* _Swapchain)
	{
		VulkanDescriptorAllocator* vkDescriptorAllocator = new VulkanDescriptorAllocator;

		if (!vkDescriptorAllocator->CreateDescriptorAllocator(_Device, _Swapchain))
			return nullptr;

		return vkDescriptorAllocator;
	}

	ISemaphore* VulkanRenderer::InstantiateSemaphore(IDevice* _Device)
	{
		VulkanSemaphore* vkSemaphore = new VulkanSemaphore;

		if (!vkSemaphore->CreateSemaphoreSync(_Device))
			return nullptr;

		return vkSemaphore;
	}

	IFence* VulkanRenderer::InstantiateFence(IDevice* _Device)
	{
		VulkanFence* vkFence = new VulkanFence;

		if (!vkFence->CreateFenceSync(_Device))
			return nullptr;

		return vkFence;
	}

	IBuffer* VulkanRenderer::CreateBuffer(IDevice* _Device, BufferType _BufferType, size_t _BufferSize)
	{
		VulkanBuffer* vkBuffer = new VulkanBuffer;

		if (!vkBuffer->CreateBuffer(_Device, _BufferType, _BufferSize))
			return nullptr;

		return vkBuffer;
	}
}