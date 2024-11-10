#include "ITexture.h"

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Resources
{
	const bool ITexture::Load(std::filesystem::path _ResourcePath)
	{
		int textWidth, textHeight, textChannels;
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* texture = stbi_load(_ResourcePath.string().c_str(), &textWidth, &textHeight, &textChannels, STBI_rgb_alpha);

		if (!texture)
		{
			DEBUG_ERROR("Failed to load texture");
			return false;
		}

		return false;
	}

	const bool ITexture::Unload()
	{
		return false;
	}
}