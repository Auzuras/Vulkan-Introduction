#include "RHI/VulkanRHI/VulkanTypes/VulkanDescriptor.h"

#include "RHI/VulkanRHI/VulkanTypes/VulkanDevice.h"

namespace Core
{
	VulkanDescriptor::~VulkanDescriptor()
	{}

	const RHI_RESULT VulkanDescriptor::CreateDescriptorLayout(IDevice* _Device)
	{
		VulkanDevice device = *_Device->CastToVulkan();

		// Descriptor Set layout binding for ubo
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		// Describe the biding of the descriptors sets (binding = 0 but for ubo samplers etc)
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		// Can specifies if it is an array of descriptor
		uboLayoutBinding.descriptorCount = 1;
		// Describes which stages can access the UBO can also be VK_SHADER_STAGE_ALL_GRAPHICS
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		// Descriptor Set layout binding for sampler
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		// Specifies a sampler already created to avoid specification
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		// Describes all the bindings possibles
		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		// Layou infos
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		// Nbr of bindings
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());;
		// Bindings data
		layoutInfo.pBindings = bindings.data();

		// Creates the Descriptor set layout
		VkResult result = vkCreateDescriptorSetLayout(device.GetLogicalDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout);

		if (result != VK_SUCCESS)
		{
			DEBUG_ERROR("Failed to create descriptor set layout, Error Code: %d", result);
			return RHI_FAILED_UNKNOWN;
		}

		return RHI_SUCCESS;
	}

	void VulkanDescriptor::CreateDescriptorSets()
	{
		//std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);

		//// Allocation infos
		//VkDescriptorSetAllocateInfo allocInfo{};
		//allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		//allocInfo.descriptorPool = m_DescriptorPool;
		//// Size and data of all the Descriptor set layout
		//allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		//allocInfo.pSetLayouts = layouts.data();

		//m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

		//// Allocates memory for all descriptor sets
		//VkResult result = vkAllocateDescriptorSets(m_LogicalDevice, &allocInfo, m_DescriptorSets.data());

		//if (result != VK_SUCCESS)
		//{
		//	DEBUG_ERROR("Failed to allocate descriptor sets, Error Code: %d", result);
		//}

		//// Specifies parameters for all descriptors
		//for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		//{
		//	// UBO info
		//	VkDescriptorBufferInfo bufferInfo{};
		//	bufferInfo.buffer = m_UniformBuffers[i];
		//	bufferInfo.offset = 0;
		//	bufferInfo.range = sizeof(UniformMVP);

		//	// Image info
		//	VkDescriptorImageInfo imageInfo{};
		//	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//	imageInfo.imageView = m_TextureImageView;
		//	imageInfo.sampler = m_TextureSampler;

		//	std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		//	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//	// Descriptor set to update
		//	descriptorWrites[0].dstSet = m_DescriptorSets[i];
		//	// Binding to update
		//	descriptorWrites[0].dstBinding = 0;
		//	descriptorWrites[0].dstArrayElement = 0;
		//	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//	descriptorWrites[0].descriptorCount = 1;
		//	descriptorWrites[0].pBufferInfo = &bufferInfo;

		//	// Update sampler
		//	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//	descriptorWrites[1].dstSet = m_DescriptorSets[i];
		//	descriptorWrites[1].dstBinding = 1;
		//	descriptorWrites[1].dstArrayElement = 0;
		//	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//	descriptorWrites[1].descriptorCount = 1;
		//	descriptorWrites[1].pImageInfo = &imageInfo;

		//	// Updates the descriptor sets
		//	vkUpdateDescriptorSets(m_LogicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		//}
	}

	void VulkanDescriptor::DestroyDescriptorSets()
	{
		//vkDestroyDescriptorSetLayout(m_LogicalDevice, m_DescriptorSetLayout, nullptr);
	}
}