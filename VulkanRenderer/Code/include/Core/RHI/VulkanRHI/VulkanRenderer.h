#pragma once

#define NOMINMAX

#include "RHI/IRendererHardware.h"

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>
#include <string>
#include <filesystem>
#include "RHI/Vertex.h"
#include "Matrices/Matrix4.h"

#include <shaderc/shaderc.hpp>

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

		/// SwapChain related methods
		
		///////////////////////////////////////////////////////////////////////

		IPipeline* InstantiatePipeline(IDevice* _Device) override;

		Resources::IShader* CompileShader(std::string _ShaderSourceCode, ShaderType _) override;
	};
}