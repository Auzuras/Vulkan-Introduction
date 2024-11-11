#pragma once

#include "RHI/RHITypes/IDescriptor.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDescriptor : public IDescriptor
	{
	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorSet m_DescriptorSets;

	public:

		~VulkanDescriptor() override;

		/// <summary>
		/// Creates all the descriptor set layout to describe the UBO or global variables
		/// </summary>
		const RHI_RESULT CreateDescriptorLayout(IDevice* _Device) override;

		/// <summary>
		/// Creates all Descriptor Sets
		/// </summary>
		void CreateDescriptorSets();
		
		void DestroyDescriptorSets();

	};
}