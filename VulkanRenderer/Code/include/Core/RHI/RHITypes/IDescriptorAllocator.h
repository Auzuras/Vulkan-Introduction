#pragma once

#include "RHI/RHITypes/RHIResult.h"

#include <vector>

namespace Core
{
	class IDevice;
	class ISwapChain;
	class IDescriptor;
	class IDescriptorLayout;
	class ITexture;
	class IBuffer;

	class IDescriptorAllocator
	{
	public:
		virtual ~IDescriptorAllocator() = default;

		virtual const RHI_RESULT CreateDescriptorAllocator(IDevice* _Device, ISwapChain* _Swapchain) = 0;
		virtual const RHI_RESULT DestroyDescriptorAllocator(IDevice* _Device) = 0;

		virtual std::vector<IDescriptor*> CreateTextureDescriptor(IDevice* _Device, int _DescriptorNbr, ITexture* _Texture, IDescriptorLayout* _Layout) = 0;
		virtual std::vector<IDescriptor*> CreateUBODescriptor(IDevice* _Device, int _DescriptorNbr, std::vector<IBuffer*>, size_t _BufferSize, IDescriptorLayout* _Layout) = 0;
	};
}