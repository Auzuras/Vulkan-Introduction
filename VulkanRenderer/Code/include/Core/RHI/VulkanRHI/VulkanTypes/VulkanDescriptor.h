#pragma once

#include "RHI/RHITypes/IDescriptor.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDescriptor : public IDescriptor
	{
	private:
		VkDescriptorSet m_DescriptorSets;

	public:

		~VulkanDescriptor() override;
		
		void DestroyDescriptorSets();

	};
}