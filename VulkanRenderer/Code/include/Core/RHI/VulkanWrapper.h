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

	struct UniformMVP
	{
		Math::Matrix4 model;
		Math::Matrix4 view;
		Math::Matrix4 projection;
	};

	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
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

		bool m_FramebufferResized = false;

		uint32_t m_CurrentFrame = 0;

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
		~VulkanWrapper() = default;

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		const bool Initialize(GLFWwindow* _Window) override;

		/// <summary>
		/// Creates a vulkan instance
		/// </summary>
		const bool CreateVulkanInstance();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const bool SetupDebugMessenger();

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
		/// 
		/// </summary>
		/// <param name="_MessageSeverity"></param>
		/// <param name="_MessageType"></param>
		/// <param name="_CallbackData"></param>
		/// <param name="_UserData"></param>
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

		void RecreateSwapChain(GLFWwindow* _Window);

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

		void CopyBuffer(VkBuffer _SourceBuffer, VkBuffer _DestinationBuffer, VkDeviceSize _Size);

		void CreateIndexBuffer();

		void CreateDescriptorSetLayout();
	};
}