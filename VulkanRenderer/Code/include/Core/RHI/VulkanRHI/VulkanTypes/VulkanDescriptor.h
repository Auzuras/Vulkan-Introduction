#pragma once

#include "RHI/RHITypes/IDescriptor.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDescriptor : public IDescriptor
	{
	private:

	public:
		VkDescriptorSet m_DescriptorSets = VK_NULL_HANDLE;

		VulkanDescriptor() = default;
		VulkanDescriptor(VkDescriptorSet _Descriptor);
		~VulkanDescriptor() override;

		inline VulkanDescriptor* CastToVulkan() override { return this; }
		inline VkDescriptorSet GetType() { return m_DescriptorSets; }
	};
}