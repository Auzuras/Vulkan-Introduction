#pragma once

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanImage
	{
	private:
		VkImage m_Image;
		VkImageView m_ImageView;
	public:

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
		/// Creates an image view object
		/// </summary>
		/// <param name="_Image">: Base image </param>
		/// <param name="_Format">: Format of the image </param>
		/// <param name="_AspectFlags">: Flags defining which aspects of the image are accessible </param>
		/// <returns></returns>
		VkImageView CreateImageView(VkImage _Image, VkFormat _Format, VkImageAspectFlags _AspectFlags);

		/// <summary>
		/// Transitions the image from one layout to new one
		/// </summary>
		/// <param name="_Image">: Image that you want to change the layout </param>
		/// <param name="_Format">: Format of the image </param>
		/// <param name="_OldLayout">: Old layout of the image </param>
		/// <param name="_NewLayout">: New layout of the image </param>
		void TransitionImageLayout(VkImage _Image, VkFormat _Format, VkImageLayout _OldLayout, VkImageLayout _NewLayout);

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

		RHI_RESULT DestroyImage();
	};
}