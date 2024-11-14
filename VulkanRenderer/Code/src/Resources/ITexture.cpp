#include "ITexture.h"

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Core
{
	const bool ITexture::Load(Core::IDevice* _Device, std::filesystem::path _ResourcePath)
	{
		int textWidth, textHeight, textChannels;
		stbi_set_flip_vertically_on_load(true);

		unsigned char* texture = stbi_load(_ResourcePath.string().c_str(), &textWidth, &textHeight, &textChannels, STBI_rgb_alpha);

		if (!texture)
		{
			DEBUG_ERROR("Failed to load texture");
			return false;
		}

		CreateTexture(_Device, texture, textWidth, textHeight);

		// Free the texture on the CPU
		stbi_image_free(texture);

		return true;
	}

	const bool ITexture::Unload(Core::IDevice* _Device)
	{
		DestroyTexture(_Device);
		return true;
	}
}