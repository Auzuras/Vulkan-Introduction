#pragma once

namespace Core
{
	enum RendererType
	{
		OPENGL,
		VULKAN,
		DIRECTX
	};

	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY
	};

	enum BufferType
	{
		RHI_VERTEX_BUFFER,
		RHI_INDEX_BUFFER,
		RHI_UNIFORM_BUFFER
	};
}