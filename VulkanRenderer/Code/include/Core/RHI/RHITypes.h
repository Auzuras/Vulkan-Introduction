#pragma once

namespace Core
{
	enum RendererType
	{
		RHI_OPENGL,
		RHI_VULKAN,
		RHI_DIRECTX
	};

	enum ShaderType
	{
		RHI_VERTEX,
		RHI_FRAGMENT,
		RHI_GEOMETRY
	};

	enum BufferType
	{
		RHI_VERTEX_BUFFER,
		RHI_INDEX_BUFFER,
		RHI_UNIFORM_BUFFER
	};

	enum DescriptorType
	{
		RHI_DESCRIPTOR_UNIRFORM,
		RHI_DESCRIPTOR_SAMPLER
	};
}