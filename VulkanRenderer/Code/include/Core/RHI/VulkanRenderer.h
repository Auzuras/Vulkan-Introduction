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
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		float iRandomRotation = 0.f;

		VkInstance m_VulkanInstance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_LogicalDevice;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		VkSurfaceKHR m_Surface;

		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;

		VkRenderPass m_RenderPass;

		VkPipeline m_GraphicsPipeline;

		std::vector<VkFramebuffer> m_SwapChainFramebuffers;

		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;

		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;

		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_IndexBufferMemory;

		std::vector<VkBuffer> m_UniformBuffers;
		std::vector<VkDeviceMemory> m_UniformBufferMemory;
		std::vector<void*> m_UniformBuffersMapped;

		VkDescriptorPool m_DescriptorPool;
		std::vector<VkDescriptorSet> m_DescriptorSets;

		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;

		VkImageView m_TextureImageView;
		VkSampler m_TextureSampler;

		bool m_FramebufferResized = false;

		uint32_t m_CurrentFrame = 0;

		VkImage m_DepthImage;
		VkDeviceMemory m_DepthImageMemory;
		VkImageView m_DepthImageView;

		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> m_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif

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
		/// Creates a vulkan instance that links the application with the API
		/// </summary>
		const bool CreateVulkanInstance();

		/// <summary>
		/// Setup debugger tool for validation layers
		/// </summary>
		/// <returns></returns>
		const bool SetupDebugMessenger();


		/// <summary>
		/// Terminates - Cleans all the informations stored in the Vulkan RHI
		/// </summary>
		/// <returns></returns>
		const bool Terminate() override;

		///////////////////////////////////////////////////////////////////////

		/// Validation layers related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Checks if validation layers are supported with the SDK
		/// </summary>
		/// <returns></returns>
		const bool CheckValidationLayerSupport();

		/// <summary>
		/// Callback method called when a validation layer should be print
		/// </summary>
		/// <param name="_MessageSeverity">: Type of severity of the message, LOG, WARN, EROOR </param>
		/// <param name="_MessageType">: Type of the message </param>
		/// <param name="_CallbackData">: Data of the message </param>
		/// <param name="_UserData">: More data from the user </param>
		/// <returns></returns>
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT _MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT _MessageType, const VkDebugUtilsMessengerCallbackDataEXT* _CallbackData, void* _UserData);

		/// <summary>
		/// Creates the debug messenger for validation layers
		/// </summary>
		/// <param name="_Instance">: Vulkan instance </param>
		/// <param name="_CreateInfo">: Creation information structure - Specifies all the informations for the debug messenger </param>
		/// <param name="_Allocator">: Pointer to a structure for custom memory allocations </param>
		/// <param name="_DebugMessenger">: Debug messenger you want to create </param>
		/// <returns></returns>
		VkResult CreateDebugUtilsMessengerEXT(VkInstance _Instance, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo, const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger);

		/// <summary>
		/// Destroys the debug messenger and the callback method
		/// </summary>
		/// <param name="_Instance">: Vulkan instance of the program </param>
		/// <param name="_DebugMessenger">: Debug messenger you want to destroy </param>
		/// <param name="_Allocator">: Pointer to a structure for custom memory allocations </param>
		void DestroyDebugUtilsMessengerEXT(VkInstance _Instance, VkDebugUtilsMessengerEXT _DebugMessenger, const VkAllocationCallbacks* _Allocator);

		/// <summary>
		/// Specifies all the information for the creation of the debug messenger
		/// </summary>
		/// <param name="_CreateInfo">: Create info structure of the debug messenger </param>
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _CreateInfo);

		///////////////////////////////////////////////////////////////////////

		/// Extensions related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Get all required extensions needed for the application (GLFW and Debug extensions)
		/// </summary>
		/// <returns></returns>
		std::vector<const char*> GetRequiredExtensions();

		///////////////////////////////////////////////////////////////////////

		/// Physical divice related methods

		///////////////////////////////////////////////////////////////////////
		
		/// <summary>
		/// Selects a GPU of the computer in order to use it
		/// </summary>
		void PickPhysicalDevice();

		/// <summary>
		/// Checks if a GPU is able to handle the operations required for our application
		/// </summary>
		/// <param name="_Device">: Physical device that will be checked </param>
		/// <returns></returns>
		const bool IsDeviceSuitable(VkPhysicalDevice _Device);

		///////////////////////////////////////////////////////////////////////

		/// Families queues related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Find all queue famillies suporting features required by the application (Graphics and presentation)
		/// </summary>
		/// <param name="_Device">: Physical device containing queue families </param>
		/// <returns></returns>
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _Device);

		///////////////////////////////////////////////////////////////////////

		/// Logical devices related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a logical device used to interface with Vulkan and our RHI
		/// </summary>
		void CreateLogicalDevice();

		///////////////////////////////////////////////////////////////////////

		/// Surface related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a surface which is going to link Vulkan with Window API
		/// </summary>
		/// <param name="_Window">: Current window context </param>
		void CreateSurface(Window* _Window);


		///////////////////////////////////////////////////////////////////////

		/// SwapChain related methods
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Checks if the extensions needed for our program are supported by our GPU
		/// </summary>
		/// <param name="_Device">: Logical device which we check extensions support </param>
		/// <returns></returns>
		const bool CheckDeviceExtensionSupport(VkPhysicalDevice _Device);

		/// <summary>
		/// Checks if the SwapChain is supported bu our GPU
		/// </summary>
		/// <param name="_Device">: Logical device which we check if the swap chain is supported </param>
		/// <returns></returns>
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _Device);

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
		void RecordCommandBuffer(VkCommandBuffer _CommandBuffer, uint32_t _ImageIndex);

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
		void UpdateUniformBuffer(uint32_t _CurrentImage);

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