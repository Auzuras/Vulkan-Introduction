#include "Model.h"

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
			//p_Descriptors[i] = Core::Renderer::GetDescriptorAllocator();
		}
	}

	void Model::Update()
	{
	}

	void Model::Draw()
	{
	}
}