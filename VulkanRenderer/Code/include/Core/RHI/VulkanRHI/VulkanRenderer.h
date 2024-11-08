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
	const int MAX_FRAMES_IN_FLIGHT = 2;

	const std::string MODEL_PATH = "Assets/Meshes/viking_room.obj";
	const std::string TEXTURE_PATH = "Assets/Textures/viking_room.png";

	struct CompilationInfos
	{
		const char* fileName;
		shaderc_shader_kind shaderKind;
		std::string* sourceCode;
		shaderc::CompileOptions options;
	};

	struct UniformMVP
	{
		 alignas(16) Math::Matrix4 model;
		 alignas(16) Math::Matrix4 view;
		 alignas(16) Math::Matrix4 projection;
	};

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transfertFamily;

		bool isComplete = false;
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanRenderer : public IRendererHardware
	{
	private:
		// SwapChain
		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;

		// Sync object
		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;

		// Uniforms
		std::vector<VkBuffer> m_UniformBuffers;
		std::vector<VkDeviceMemory> m_UniformBufferMemory;
		std::vector<void*> m_UniformBuffersMapped;

		// Descriptors
		VkDescriptorPool m_DescriptorPool;
		std::vector<VkDescriptorSet> m_DescriptorSets;

		// Texture
		VkImage m_TextureImage;
		VkImageView m_TextureImageView;

		VkDeviceMemory m_TextureImageMemory;
		VkSampler m_TextureSampler;

		bool m_FramebufferResized = false;

		uint32_t m_CurrentFrame = 0;

		VkImage m_DepthImage;
		VkDeviceMemory m_DepthImageMemory;
		VkImageView m_DepthImageView;

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

		/// Graphics pipeline related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Reads an already compiled SpirV shader
		/// </summary>
		/// <param name="_FilePath">: Path to the SpirV shader </param>
		/// <returns></returns>
		std::vector<char> ReadSpirVShader(const std::filesystem::path& _FilePath);

		///////////////////////////////////////////////////////////////////////

		/// Shader related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a shader module object with a shader source code
		/// </summary>
		/// <param name="_ShaderSourceCode">: Source code of the shader </param>
		/// <returns></returns>
		VkShaderModule CreateShaderModule(const std::vector<uint32_t>& _ShaderSourceCode);

		/// <summary>
		/// Compiles a shader in run time according to the path of the shader and its type
		/// </summary>
		/// <param name="_ShaderPath">: Path of the shader </param>
		/// <param name="_ShaderType">: Type of the shader (Vertex / Geometry / Fragment)  </param>
		std::vector<uint32_t> CompileShader(std::filesystem::path _ShaderPath, ShaderType _ShaderType);

		///////////////////////////////////////////////////////////////////////

		/// Draw related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Executes all drawing commands and presentation commands
		/// </summary>
		void DrawFrame(Window* _Window) override;

		/// <summary>
		/// Creates all synchronization objects needed for rendering
		/// </summary>
		void CreateSyncObjects();

		/// <summary>
		/// Creates an uniform buffer
		/// </summary>
		void CreateUniformBuffers();

		/// <summary>
		/// Updates the uniform buffer
		/// </summary>
		/// <param name="_CurrentImage">: Current image updated </param>
		void UpdateUniformBuffer(uint32_t _CurrentImage, Math::Matrix4 _ModelMatrix);

		///////////////////////////////////////////////////////////////////////

		/// Texture related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Allows to move an image from a src buffer to a dest image
		/// </summary>
		/// <param name="_Buffer">: Source buffer </param>
		/// <param name="_Image">: Destination image </param>
		/// <param name="_Width">: Width of the image </param>
		/// <param name="_Height">: Height of the image </param>
		void CopyBufferToImage(VkBuffer _Buffer, VkImage _Image, uint32_t _Width, uint32_t _Height);

		///////////////////////////////////////////////////////////////////////

		/// Depth related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a depth texture to store the depth buffer
		/// </summary>
		void CreateDepthRessources();

		/// <summary>
		/// Find a supported Format between many candidates
		/// </summary>
		/// <param name="_Candidates">: Formats that we want to check the support </param>
		/// <param name="_Tiling">: Type of tiling </param>
		/// <param name="_Features">: Flags of features we want to support </param>
		/// <returns></returns>
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& _Candidates, VkImageTiling _Tiling, VkFormatFeatureFlags _Features);

		/// <summary>
		/// Checks if depth texture format are supported for depth attachment
		/// </summary>
		/// <returns></returns>
		VkFormat FindDepthFormat();

		/// <summary>
		/// Checks is a format also contains a stencil component
		/// </summary>
		/// <param name="_Format">: Format that you want to check </param>
		/// <returns></returns>
		bool HasStencilComponent(VkFormat _Format);
	};
}