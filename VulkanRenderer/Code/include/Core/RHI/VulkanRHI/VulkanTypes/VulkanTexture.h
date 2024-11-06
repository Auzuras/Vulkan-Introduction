#pragma once

#include "ITexture.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanImage.h"

namespace Core
{
	class VulkanTexture : public Resources::ITexture
	{
	private:
		VulkanImage m_TextureImage;

		VkDeviceMemory m_TextureImageMemory;
		VkSampler m_TextureSampler;

	public:
	};
}