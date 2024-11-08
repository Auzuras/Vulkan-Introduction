#pragma once

#define NOMINMAX

#include "RHI/IRendererHardware.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanTypesWrapper.h"

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

		/// <summary>
		/// Creates a device
		/// </summary>
		/// <returns></returns>
		IDevice* CreateDevice();

		///// <summary>
		///// Reads an already compiled SpirV shader
		///// </summary>
		///// <param name="_FilePath">: Path to the SpirV shader </param>
		///// <returns></returns>
		//std::vector<char> ReadSpirVShader(const std::filesystem::path& _FilePath);

		///// <summary>
		///// Creates a shader module object with a shader source code
		///// </summary>
		///// <param name="_ShaderSourceCode">: Source code of the shader </param>
		///// <returns></returns>
		//VkShaderModule CreateShaderModule(const std::vector<uint32_t>& _ShaderSourceCode);

		///// <summary>
		///// Compiles a shader in run time according to the path of the shader and its type
		///// </summary>
		///// <param name="_ShaderPath">: Path of the shader </param>
		///// <param name="_ShaderType">: Type of the shader (Vertex / Geometry / Fragment)  </param>
		//std::vector<uint32_t> CompileShader(std::filesystem::path _ShaderPath, ShaderType _ShaderType);

		/// <summary>
		/// Allows to move an image from a src buffer to a dest image
		/// </summary>
		/// <param name="_Buffer">: Source buffer </param>
		/// <param name="_Image">: Destination image </param>
		/// <param name="_Width">: Width of the image </param>
		/// <param name="_Height">: Height of the image </param>
		void CopyBufferToImage(VkBuffer _Buffer, VkImage _Image, uint32_t _Width, uint32_t _Height);
	};
}