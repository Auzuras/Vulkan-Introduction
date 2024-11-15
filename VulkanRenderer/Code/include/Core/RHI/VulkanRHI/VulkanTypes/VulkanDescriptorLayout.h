#pragma once

#include "RHI/RHITypes/IDescriptorLayout.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDescriptorLayout : public IDescriptorLayout
	{
	private:
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;

	public:

		~VulkanDescriptorLayout() override;

		const RHI_RESULT CreateDescriptorSetLayout(IDevice* _Device, std::vector<DescriptorLayoutInfos> _Infos) override;
		const void DestroyDescriptorSetLayout(IDevice* _Device) override;

		inline VulkanDescriptorLayout* CastToVulkan() override { return this; }

		inline VkDescriptorSetLayout GetType() { return m_DescriptorSetLayout; }
	};
}