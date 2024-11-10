#pragma once

#include "ITexture.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanImage.h"

namespace Core
{
	class VulkanTexture : public Resources::ITexture
	{
	private:
		//VulkanImage m_TextureImage;

		VkDeviceMemory m_TextureImageMemory;
		VkSampler m_TextureSampler;

	public:
		/// <summary>
		/// Creates an image that can be used in Vulkan with a texture loaded with STB Image
		/// </summary>
		void CreateTextureImage();

		/// <summary>
		/// Creates an Image view for a texture in order to access it
		/// </summary>
		void CreateTextureImageView();

		/// <summary>
		/// Creates a texture sampler to access a texture in a shader
		/// </summary>
		void CreateTextureSampler();

		RHI_RESULT DestroyTexture();

		const bool Load(std::filesystem::path _ResourcePath) override;
		const bool Unload() override;
	};
}