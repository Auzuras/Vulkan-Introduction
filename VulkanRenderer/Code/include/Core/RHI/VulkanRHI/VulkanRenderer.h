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

		// Pipeline
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;
		VkRenderPass m_RenderPass;
		VkPipeline m_GraphicsPipeline;

		// Comands
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		// Buffers
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;

		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_IndexBufferMemory;

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

		/// SwapChain related methods
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Selects the best format available for the swapchain
		/// </summary>
		/// <param name="_AvailableFormats">: All the availables formats of the swap chain</param>
		/// <returns></returns>
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _AvailableFormats);
		
		/// <summary>
		/// Selects the best presentation mode for the swap chain
		/// </summary>
		/// <param name="_AvailablePresentModes">: All the presentation modes of the swap chain</param>
		/// <returns></returns>
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _AvailablePresentModes);

		/// <summary>
		/// Selects the image format for the swap chain
		/// </summary>
		/// <param name="capabilities">: Swap chain capabilities availables </param>
		/// <returns></returns>
		VkExtent2D ChooseSwapExtent(Window* _Window, const VkSurfaceCapabilitiesKHR& _Capabilities);

		/// <summary>
		/// Creates the Swap chain for our program
		/// </summary>
		/// <param name="_Window">: Current window context </param>
		void CreateSwapChain(Window* _Window);

		/// <summary>
		/// Recreates the Swap chain
		/// </summary>
		/// <param name="_Window">: Current window context </param>
		void RecreateSwapChain(Window* _Window);

		/// <summary>
		/// Destroys all data linked to the swap chain and the swap chain
		/// </summary>
		void CleanSwapChain();

		///////////////////////////////////////////////////////////////////////

		/// Image view related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates the image views of the swap chain
		/// </summary>
		void CreateSwapChainImageViews();

		///////////////////////////////////////////////////////////////////////

		/// Graphics pipeline related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a simple graphics pipeline to draw on screen
		/// </summary>
		void CreateGraphicsPipeline();

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

		/// Render pass related methods
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a render pass object that will descibes the attachments of a framebuffer
		/// </summary>
		void CreateRenderPass();

		///////////////////////////////////////////////////////////////////////

		/// Frame buffers related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates all the swap chain frames buffers for each image view
		/// </summary>
		void CreateSwapChainFramebuffers();

		///////////////////////////////////////////////////////////////////////

		/// Commands related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a command pool that will manage the memory for command buffers
		/// </summary>
		void CreateCommandPool();

		/// <summary>
		/// Creates all the command buffers
		/// </summary>
		void CreateCommandBuffers();

		/// <summary>
		/// Records all the commands for a command buffer
		/// </summary>
		/// <param name="_CommandBuffer">: Command buffer used </param>
		/// <param name="_ImageIndex">: Current image </param>
		void RecordDrawCommandBuffer(VkCommandBuffer _CommandBuffer, uint32_t _ImageIndex);

		void DrawCommandBuffer(VkCommandBuffer _CommandBuffer);

		void EndDrawCommandBuffer(VkCommandBuffer _CommandBuffer);

		/// <summary>
		/// Creates a temporary command buffer for a single command
		/// </summary>
		/// <returns></returns>
		VkCommandBuffer BeginSingleTimeCommands();

		/// <summary>
		/// Ends and destroy a temporary command buffer for a single command
		/// </summary>
		/// <param name="_CommandBuffer">: Command buffer to destroy </param>
		void EndSingleTimeCommands(VkCommandBuffer _CommandBuffer);

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
		/// Method called when the window is resized manually
		/// </summary>
		/// <param name="_Window">: Current window context </param>
		/// <param name="_Width">: Window width </param>
		/// <param name="_Height">: Window height </param>
		static void FrameBufferResizeCallback(GLFWwindow* _Window, int _Width, int _Height);

		/// <summary>
		/// Creates and allocate a buffer in memory
		/// </summary>
		/// <param name="_Size">: Size of the buffer </param>
		/// <param name="_Usage">: Type of usage of the buffer </param>
		/// <param name="_Properties">: Buffer properties </param>
		/// <param name="_Buffer">: Buffer you want to create and store </param>
		/// <param name="_BufferMemory">: Buffer memory you want to allocate </param>
		void CreateBuffer(VkDeviceSize _Size, VkBufferUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory);

		/// <summary>
		/// Creates a vertex buffer
		/// </summary>
		void CreateVertexBuffer();

		/// <summary>
		/// Selects the best memory type for an allocation
		/// </summary>
		/// <param name="_TypeFilter">: Type of memory needed by a buffer </param>
		/// <param name="_Properties">: Properties needed for the buffer </param>
		/// <returns></returns>
		uint32_t FindMemoryType(uint32_t _TypeFilter, VkMemoryPropertyFlags _Properties);

		/// <summary>
		/// Copies a buffer from a source buffer to a destination buffer
		/// </summary>
		/// <param name="_SourceBuffer">: First buffer you want to copy </param>
		/// <param name="_DestinationBuffer">: Second buffer receiving the first one </param>
		/// <param name="_Size">: Size of the buffer </param>
		void CopyBuffer(VkBuffer _SourceBuffer, VkBuffer _DestinationBuffer, VkDeviceSize _Size);

		/// <summary>
		/// Creates an index buffer
		/// </summary>
		void CreateIndexBuffer();

		/// <summary>
		/// Creates all the descriptor set layout to describe the UBO or global variables
		/// </summary>
		void CreateDescriptorSetLayout();

		/// <summary>
		/// Creates an uniform buffer
		/// </summary>
		void CreateUniformBuffers();

		/// <summary>
		/// Updates the uniform buffer
		/// </summary>
		/// <param name="_CurrentImage">: Current image updated </param>
		void UpdateUniformBuffer(uint32_t _CurrentImage, Math::Matrix4 _ModelMatrix);

		/// <summary>
		/// Creates all descriptor pool
		/// </summary>
		void CreateDescriptorPool();

		/// <summary>
		/// Creates all Descriptor Sets
		/// </summary>
		void CreateDescriptorSets();

		///////////////////////////////////////////////////////////////////////

		/// Texture related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates an image view object
		/// </summary>
		/// <param name="_Image">: Base image </param>
		/// <param name="_Format">: Format of the image </param>
		/// <param name="_AspectFlags">: Flags defining which aspects of the image are accessible </param>
		/// <returns></returns>
		VkImageView CreateImageView(VkImage _Image, VkFormat _Format, VkImageAspectFlags _AspectFlags);

		/// <summary>
		/// Creates an image that can be used in Vulkan with a texture loaded with STB Image
		/// </summary>
		void CreateTextureImage();

		/// <summary>
		/// Creates and allocates the memory for an image
		/// </summary>
		/// <param name="_Width">: Width of the image </param>
		/// <param name="_Height">: Height of the image </param>
		/// <param name="_Format">: Color format of the image </param>
		/// <param name="_Tiling">: Format how the data is stored </param>
		/// <param name="_Usage">: Usage type of the image</param>
		/// <param name="_Properties">: More properties about the image </param>
		/// <param name="_Image">: Variable where you want to keep the image </param>
		/// <param name="_ImageMemory">: Variable for the memory of the image </param>
		void CreateImage(uint32_t _Width, uint32_t _Height, VkFormat _Format, VkImageTiling _Tiling, VkImageUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkImage& _Image, VkDeviceMemory& _ImageMemory);

		/// <summary>
		/// Transitions the image from one layout to new one
		/// </summary>
		/// <param name="_Image">: Image that you want to change the layout </param>
		/// <param name="_Format">: Format of the image </param>
		/// <param name="_OldLayout">: Old layout of the image </param>
		/// <param name="_NewLayout">: New layout of the image </param>
		void TransitionImageLayout(VkImage _Image, VkFormat _Format, VkImageLayout _OldLayout, VkImageLayout _NewLayout);

		/// <summary>
		/// Allows to move an image from a src buffer to a dest image
		/// </summary>
		/// <param name="_Buffer">: Source buffer </param>
		/// <param name="_Image">: Destination image </param>
		/// <param name="_Width">: Width of the image </param>
		/// <param name="_Height">: Height of the image </param>
		void CopyBufferToImage(VkBuffer _Buffer, VkImage _Image, uint32_t _Width, uint32_t _Height);

		/// <summary>
		/// Creates an Image view for a texture in order to access it
		/// </summary>
		void CreateTextureImageView();

		/// <summary>
		/// Creates a texture sampler to access a texture in a shader
		/// </summary>
		void CreateTextureSampler();

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

		///////////////////////////////////////////////////////////////////////

		/// Model related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Loads an OBJ 3D model
		/// </summary>
		void LoadModel();
	};
}