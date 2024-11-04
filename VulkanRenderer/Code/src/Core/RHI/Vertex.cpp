#include "RHI/Vertex.h"

namespace Core
{
	VkVertexInputBindingDescription Vertex::GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		// Only one vertex so bind to 0
		bindingDescription.binding = 0;
		// Stride or size of the complete type
		bindingDescription.stride = sizeof(Vertex);
		// Input rate VK_VERTEX_INPUT_RATE_VERTEX -> send the next data after each summit
		// VK_VERTEX_INPUT_RATE_INSTANCE -> send the next data after each instance
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 3> Vertex::GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		// The binding in the shader
		attributeDescriptions[0].location = 0;
		// Format of the data here Vector3
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		// The offset between each data
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, textCoord);

		return attributeDescriptions;
	}
}