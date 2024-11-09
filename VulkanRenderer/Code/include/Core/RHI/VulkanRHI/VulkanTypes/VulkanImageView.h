#pragma once

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanImageView
	{
	private:
		VkImageView m_ImageView;

	public:
		/// <summary>
		/// Creates an image view object
		/// </summary>
		/// <param name="_Device">: Device </param>
		/// <param name="_Image">: Base image </param>
		/// <param name="_Format">: Format of the image </param>
		/// <param name="_AspectFlags">: Flags defining which aspects of the image are accessible </param>
		/// <returns></returns>
		const RHI_RESULT CreateImageView(VkDevice _Device, VkImage _Image, VkFormat _Format, VkImageAspectFlags _AspectFlags);

		const RHI_RESULT DestroyImageView(VkDevice _Device);
	};
}