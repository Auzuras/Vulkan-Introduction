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

		/// <summary>
		/// Describes how a complete vertex is in memory
		/// </summary>
		/// <returns></returns>
		static VkVertexInputBindingDescription GetBindingDescription();

		/// <summary>
		/// Describes each attributes of the vertex
		/// </summary>
		/// <returns></returns>
		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
	};
}