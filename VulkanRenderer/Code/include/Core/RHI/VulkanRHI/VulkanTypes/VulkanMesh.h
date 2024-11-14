#pragma once

#include "IMesh.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"
#include "RHI/VulkanRHI/VulkanTypes/VulkanBuffer.h"

namespace Core
{
	class VulkanMesh : public IMesh
	{
	private:
		VulkanBuffer m_VertexBuffer;
		VulkanBuffer m_IndexBuffer;

		size_t m_IndexNbr = 0;

	public:

		RHI_RESULT CreateVertexBuffer(Core::IDevice* _Device, std::vector<Vertex>& _VerticesList) override;
		RHI_RESULT CreateIndexBuffer(Core::IDevice* _Device, std::vector<uint32_t>& _IndicesList) override;

		RHI_RESULT DestroyBuffers(Core::IDevice* _Device) override;

		inline VulkanMesh* CastToVulkan() override { return this; }
		inline VulkanBuffer GetVertexBuffer() { return m_VertexBuffer; }
		inline VulkanBuffer GetIndexBuffer() { return m_IndexBuffer; }
		inline size_t GetIndexNumber() { return m_IndexNbr; }
	};
}