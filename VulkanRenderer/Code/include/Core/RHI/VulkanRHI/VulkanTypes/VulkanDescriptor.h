#pragma once

#include "RHI/RHITypes/IDescriptor.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDescriptor : public IDescriptor
	{
	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSet m_DescriptorSets;

	public:

		/// <summary>
		/// Creates all the descriptor set layout to describe the UBO or global variables
		/// </summary>
		void CreateDescriptorSetLayout();

		/// <summary>
		/// Creates all descriptor pool
		/// </summary>
		void CreateDescriptorPool();

		/// <summary>
		/// Creates all Descriptor Sets
		/// </summary>
		void CreateDescriptorSets();

	};
}