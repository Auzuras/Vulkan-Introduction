#pragma once

#include "ITexture.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanImage.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanImageView.h"

namespace Core
{
	class VulkanTexture : public ITexture
	{
	private:
		VulkanImage m_TextureImage;
		VulkanImageView m_TextureImageView;
		VkDeviceMemory m_TextureImageMemory;

		VkSampler m_TextureSampler;

		/// <summary>
		/// Creates an image that can be used in Vulkan with a texture loaded with STB Image
		/// </summary>
		void CreateTextureImage(VulkanDevice* _Device, unsigned char* _TextureData, int _Width, int _Height);

		/// <summary>
		/// Creates an Image view for a texture in order to access it
		/// </summary>
		void CreateTextureImageView(VulkanDevice* _Device);

		/// <summary>
		/// Creates a texture sampler to access a texture in a shader
		/// </summary>
		void CreateTextureSampler(VulkanDevice* _Device);


	public:
		RHI_RESULT CreateTexture(IDevice* _Device, unsigned char* _TextureData, int _Width, int _Height) override;
		RHI_RESULT DestroyTexture(IDevice* _Device) override;
	};
}