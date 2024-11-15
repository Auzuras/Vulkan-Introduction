#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptor.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

namespace Core
{
	VulkanDescriptor::VulkanDescriptor(VkDescriptorSet _Descriptor)
		:m_DescriptorSets(_Descriptor)
	{} 

	VulkanDescriptor::~VulkanDescriptor()
	{}
}