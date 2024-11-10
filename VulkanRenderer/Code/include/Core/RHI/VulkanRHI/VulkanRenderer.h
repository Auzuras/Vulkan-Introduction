#pragma once

#define NOMINMAX

#include "RHI/IRendererHardware.h"

#include <vulkan/vulkan.h>
#include <shaderc/shaderc.hpp>

#include <vector>
#include <optional>
#include <string>
#include <filesystem>

#include "RHI/Vertex.h"
#include "Matrices/Matrix4.h"


namespace Core
{
	class VulkanRenderer : public IRendererHardware
	{
	public:
		VulkanRenderer() = default;
		~VulkanRenderer() override;

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Initialize the Vulkan RHI
		/// </summary>
		/// <param name="_Window">: current window </param>
		/// <returns></returns>
		const bool Initialize(Window* _Window) override;

		/// <summary>
		/// Terminates - Cleans all the informations stored in the Vulkan RHI
		/// </summary>
		/// <returns></returns>
		const bool Terminate() override;

		///////////////////////////////////////////////////////////////////////

		/// Device related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a device
		/// </summary>
		/// <returns></returns>
		IDevice* InstanciateDevice(Window* _Window) override;

		///////////////////////////////////////////////////////////////////////

		/// SwapChain related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		ISwapChain* InstantiateSwapChain(Window* _Window, IDevice* _Device) override;

		///////////////////////////////////////////////////////////////////////

		/// Pipeline related methods
		
		///////////////////////////////////////////////////////////////////////

		IPipeline* InstantiatePipeline(IDevice* _Device, ISwapChain* _Swapchain) override;

		///////////////////////////////////////////////////////////////////////

		/// Command allocator related methods

		///////////////////////////////////////////////////////////////////////

		ICommandAllocator* InstantiateCommandAllocator(IDevice* _Device) override;

		///////////////////////////////////////////////////////////////////////

		/// Shader related methods

		///////////////////////////////////////////////////////////////////////

		Resources::IShader* CreateShader() override;
		Resources::IShader* CompileShader(IDevice* _Device, const char* _ShaderName, std::string _ShaderSourceCode, ShaderType _ShaderType) override;
	};
}