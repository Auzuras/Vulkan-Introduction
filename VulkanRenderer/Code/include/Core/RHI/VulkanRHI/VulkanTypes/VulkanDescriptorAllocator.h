#pragma once

#include "RHI/RHITypes/IDescriptorAllocator.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanDescriptorAllocator : public IDescriptorAllocator
	{
	private:
		VkDescriptorPool m_DescriptorPool;

	public:

		~VulkanDescriptorAllocator() override;

		/// <summary>
		/// Creates all descriptor pool
		/// </summary>
		const RHI_RESULT CreateDescriptorAllocator(IDevice* _Device, ISwapChain* _Swapchain) override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const RHI_RESULT DestroyDescriptorAllocator(IDevice* _Device) override;

		std::vector<IDescriptor*> CreateTextureDescriptor(IDevice* _Device, int _DescriptorNbr, ITexture* _Texture, IDescriptorLayout* _Layout) override;
		std::vector<IDescriptor*> CreateUBODescriptor(IDevice* _Device, int _DescriptorNbr, std::vector<IBuffer*>, size_t _BufferSize, IDescriptorLayout* _Layout) override;
	};
}