#pragma once

#include "Vectors/Vector3.h"
#include "Vectors/Vector2.h"
#include <vulkan/vulkan.h>

#include <array>

namespace Core
{
	/// <summary>
	/// Vertex structure
	/// </summary>
	struct Vertex
	{
		Math::Vector3 position;
		Math::Vector3 color;
		Math::Vector2 textCoord;

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
	};
}