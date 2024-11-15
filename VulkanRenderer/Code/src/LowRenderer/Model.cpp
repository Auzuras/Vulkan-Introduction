#include "Model.h"

#include "Renderer.h"

namespace LowRenderer
{
	Model::Model(Core::IMesh* _Mesh, Core::ITexture* _Texture)
		:m_Mesh(_Mesh), m_Texture(_Texture)
	{
		p_UniformBuffers.resize(Core::Renderer::MAX_FRAMES_IN_FLIGHT);
		p_Descriptors.resize(Core::Renderer::MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < Core::Renderer::MAX_FRAMES_IN_FLIGHT; ++i)
		{
			p_UniformBuffers[i] = Core::Renderer::GetRHI()->CreateBuffer(Core::Renderer::GetDevice(), Core::BufferType::RHI_UNIFORM_BUFFER, sizeof(Math::Matrix4));
		}

		p_Descriptors = Core::Renderer::GetDescriptorAllocator()->CreateUBODescriptor(Core::Renderer::GetDevice(), 
			Core::Renderer::MAX_FRAMES_IN_FLIGHT, p_UniformBuffers, 
			sizeof(Math::Matrix4), Core::Renderer::GetPipeline()->GetDescriptorLayouts()[0]);
	}

	void Model::DestroyDescriptors()
	{
		for (Core::IBuffer* buffer : p_UniformBuffers)
		{
			Core::Renderer::GetRHI()->DestroyBuffer(buffer, Core::Renderer::GetDevice());
		}

		for (Core::IDescriptor* descriptor : p_Descriptors)
		{
			delete descriptor;
			descriptor = nullptr;
		}
	}

	void Model::Update()
	{
	}

	void Model::Draw()
	{
	}
}