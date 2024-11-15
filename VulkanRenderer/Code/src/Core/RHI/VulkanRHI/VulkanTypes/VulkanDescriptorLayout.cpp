#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptorLayout.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"
#include <iostream>

namespace Core
{
	VulkanDescriptorLayout::~VulkanDescriptorLayout()
	{
	}
	const RHI_RESULT VulkanDescriptorLayout::CreateDescriptorSetLayout(IDevice* _Device, std::vector<DescriptorLayoutInfos> _Infos)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		for (size_t i = 0; i < _Infos.size(); ++i)
		{
			// Descriptor Set layout binding for ubo
			VkDescriptorSetLayoutBinding layoutBinding{};
			// Describe the biding of the descriptors sets (binding = 0 but for ubo samplers etc)
			layoutBinding.binding = static_cast<uint32_t>(_Infos[i].binding);

			switch (_Infos[i].type)
			{
			default:
			case RHI_DESCRIPTOR_UNIRFORM:
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			case RHI_DESCRIPTOR_SAMPLER:
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				break;
			}

			// Can specifies if it is an array of descriptor
			layoutBinding.descriptorCount = 1;

			// Describes which stages can access the UBO can also be VK_SHADER_STAGE_ALL_GRAPHICS
			switch (_Infos[i].entryShader)
			{
			default:
			case RHI_VERTEX:
				layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case RHI_FRAGMENT:
				layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			}

			// Layout info
			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			// Nbr of bindings
			layoutInfo.bindingCount = 1;
			// Bindings data
			layoutInfo.pBindings = &layoutBinding;

			// Creates the Descriptor set layout
			VkResult result = vkCreateDescriptorSetLayout(device.GetLogicalDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout);

			if (result != VK_SUCCESS)
			{
				DEBUG_ERROR("Failed to create descriptor set layout, Error Code: %d", result);
				return RHI_FAILED_UNKNOWN;
			}
		}

		return RHI_SUCCESS;
	}

	const void VulkanDescriptorLayout::DestroyDescriptorSetLayout(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		vkDestroyDescriptorSetLayout(device.GetLogicalDevice(), m_DescriptorSetLayout, nullptr);
	}
}