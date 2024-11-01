#pragma once

#define NOMINMAX

#include "RHI/RHI.h"
#include <vulkan/vulkan.h>

#include <vector>
#include <optional>
#include <string>
#include <filesystem>
#include "RHI/Vertex.h"
#include "Matrices/Matrix4.h"

namespace Core
{
	const int MAX_FRAMES_IN_FLIGHT = 2;

	const std::string MODEL_PATH = "Assets/Meshes/viking_room.obj";
	const std::string TEXTURE_PATH = "Assets/Textures/viking_room.png";

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

	class VulkanWrapper : public RHI
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
		VulkanWrapper() = default;
		~VulkanWrapper() override;

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Initialize the Vulkan RHI
		/// </summary>
		/// <param name="_Window">: current window </param>
		/// <returns></returns>
		const bool Initialize(GLFWwindow* _Window) override;

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
		/// 
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
		/// 
		/// </summary>
		/// <param name="_Instance"></param>
		/// <param name="_CreateInfo"></param>
		/// <param name="_Allocator"></param>
		/// <param name="_DebugMessenger"></param>
		/// <returns></returns>
		VkResult CreateDebugUtilsMessengerEXT(VkInstance _Instance, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo, const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Instance"></param>
		/// <param name="_DebugMessenger"></param>
		/// <param name="_Allocator"></param>
		void DestroyDebugUtilsMessengerEXT(VkInstance _Instance, VkDebugUtilsMessengerEXT _DebugMessenger, const VkAllocationCallbacks* _Allocator);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_CreateInfo"></param>
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _CreateInfo);

		///////////////////////////////////////////////////////////////////////

		/// Extensions related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::vector<const char*> GetRequiredExtensions();

		///////////////////////////////////////////////////////////////////////

		/// Physical divice related methods

		///////////////////////////////////////////////////////////////////////
		
		/// <summary>
		/// 
		/// </summary>
		void PickPhysicalDevice();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Device"></param>
		/// <returns></returns>
		const bool IsDeviceSuitable(VkPhysicalDevice _Device);

		///////////////////////////////////////////////////////////////////////

		/// Families queues related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Device"></param>
		/// <returns></returns>
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _Device);

		///////////////////////////////////////////////////////////////////////

		/// Logical devices related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void CreateLogicalDevice();

		///////////////////////////////////////////////////////////////////////

		/// Surface related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Window"></param>
		void CreateSurface(GLFWwindow* _Window);


		///////////////////////////////////////////////////////////////////////

		/// SwapChain related methods
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Device"></param>
		/// <returns></returns>
		const bool CheckDeviceExtensionSupport(VkPhysicalDevice _Device);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Device"></param>
		/// <returns></returns>
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _Device);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_AvailableFormats"></param>
		/// <returns></returns>
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _AvailableFormats);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="_AvailablePresentModes"></param>
		/// <returns></returns>
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _AvailablePresentModes);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="capabilities"></param>
		/// <returns></returns>
		VkExtent2D ChooseSwapExtent(GLFWwindow* _Window, const VkSurfaceCapabilitiesKHR& _Capabilities);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Window"></param>
		void CreateSwapChain(GLFWwindow* _Window);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Window"></param>
		void RecreateSwapChain(GLFWwindow* _Window);

		/// <summary>
		/// 
		/// </summary>
		void CleanSwapChain();

		///////////////////////////////////////////////////////////////////////

		/// Image view related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void CreateImageViews();

		///////////////////////////////////////////////////////////////////////

		/// Graphics pipeline related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void CreateGraphicsPipeline();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_FilePath"></param>
		/// <returns></returns>
		std::vector<char> ReadShader(const std::filesystem::path& _FilePath);

		///////////////////////////////////////////////////////////////////////

		/// Shader related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_ShaderSourceCode"></param>
		/// <returns></returns>
		VkShaderModule CreateShaderModule(const std::vector<char>& _ShaderSourceCode);

		///////////////////////////////////////////////////////////////////////

		/// Render pass related methods
		
		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void CreateRenderPass();

		///////////////////////////////////////////////////////////////////////

		/// Render pass related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void CreateFramebuffers();

		///////////////////////////////////////////////////////////////////////

		/// Commands related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void CreateCommandPool();

		/// <summary>
		/// 
		/// </summary>
		void CreateCommandBuffers();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_CommandBuffer"></param>
		/// <param name="_ImageIndex"></param>
		void RecordCommandBuffer(VkCommandBuffer _CommandBuffer, uint32_t _ImageIndex);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		VkCommandBuffer BeginSingleTimeCommands();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_CommandBuffer"></param>
		void EndSingleTimeCommands(VkCommandBuffer _CommandBuffer);

		///////////////////////////////////////////////////////////////////////

		/// Draw related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void DrawFrame(GLFWwindow* _Window) override;

		/// <summary>
		/// 
		/// </summary>
		void CreateSyncObjects();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Window"></param>
		/// <param name="_Width"></param>
		/// <param name="_Height"></param>
		static void FrameBufferResizeCallback(GLFWwindow* _Window, int _Width, int _Height);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Size"></param>
		/// <param name="_Usage"></param>
		/// <param name="_Properties"></param>
		/// <param name="_Buffer"></param>
		/// <param name="_BufferMemory"></param>
		void CreateBuffer(VkDeviceSize _Size, VkBufferUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkBuffer& _Buffer, VkDeviceMemory& _BufferMemory);

		/// <summary>
		/// 
		/// </summary>
		void CreateVertexBuffer();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_TypeFilter"></param>
		/// <param name="_Properties"></param>
		/// <returns></returns>
		uint32_t FindMemoryType(uint32_t _TypeFilter, VkMemoryPropertyFlags _Properties);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_SourceBuffer"></param>
		/// <param name="_DestinationBuffer"></param>
		/// <param name="_Size"></param>
		void CopyBuffer(VkBuffer _SourceBuffer, VkBuffer _DestinationBuffer, VkDeviceSize _Size);

		/// <summary>
		/// 
		/// </summary>
		void CreateIndexBuffer();

		/// <summary>
		/// 
		/// </summary>
		void CreateDescriptorSetLayout();

		/// <summary>
		/// 
		/// </summary>
		void CreateUniformBuffers();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_CurrentImage"></param>
		void UpdateUniformBuffer(uint32_t _CurrentImage);

		/// <summary>
		/// 
		/// </summary>
		void CreateDescriptorPool();

		/// <summary>
		/// 
		/// </summary>
		void CreateDescriptorSets();

		///////////////////////////////////////////////////////////////////////

		/// Texture related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Image"></param>
		/// <param name="_Format"></param>
		/// <param name="_AspectFlags"></param>
		/// <returns></returns>
		VkImageView CreateImageView(VkImage _Image, VkFormat _Format, VkImageAspectFlags _AspectFlags);

		/// <summary>
		/// 
		/// </summary>
		void CreateTextureImage();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Width"></param>
		/// <param name="_Height"></param>
		/// <param name="_Format"></param>
		/// <param name="_Tiling"></param>
		/// <param name="_Usage"></param>
		/// <param name="_Properties"></param>
		/// <param name="_Image"></param>
		/// <param name="_ImageMemory"></param>
		void CreateImage(uint32_t _Width, uint32_t _Height, VkFormat _Format, VkImageTiling _Tiling, VkImageUsageFlags _Usage, VkMemoryPropertyFlags _Properties, VkImage& _Image, VkDeviceMemory& _ImageMemory);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Image"></param>
		/// <param name="_Format"></param>
		/// <param name="_OldLayout"></param>
		/// <param name="_NewLayout"></param>
		void TransitionImageLayout(VkImage _Image, VkFormat _Format, VkImageLayout _OldLayout, VkImageLayout _NewLayout);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Buffer"></param>
		/// <param name="_Image"></param>
		/// <param name="_Width"></param>
		/// <param name="_Height"></param>
		void CopyBufferToImage(VkBuffer _Buffer, VkImage _Image, uint32_t _Width, uint32_t _Height);

		/// <summary>
		/// 
		/// </summary>
		void CreateTextureImageView();

		/// <summary>
		/// 
		/// </summary>
		void CreateTextureSampler();

		///////////////////////////////////////////////////////////////////////

		/// Depth related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void CreateDepthRessources();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Candidates"></param>
		/// <param name="_Tiling"></param>
		/// <param name="_Features"></param>
		/// <returns></returns>
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& _Candidates, VkImageTiling _Tiling, VkFormatFeatureFlags _Features);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		VkFormat FindDepthFormat();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Format"></param>
		/// <returns></returns>
		bool HasStencilComponent(VkFormat _Format);

		///////////////////////////////////////////////////////////////////////

		/// Model related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		void LoadModel();
	};
}