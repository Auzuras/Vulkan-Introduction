#pragma once

#include "RHI/RHITypes/RHIResult.h"
#include "RHI/RHITypes.h"

namespace Core
{
	class IDevice;
	class VulkanBuffer;

	class IBuffer
	{
	private:
	public:
		virtual ~IBuffer() = default;

		virtual const RHI_RESULT CreateBuffer(IDevice* _Device, BufferType _BufferType, size_t _BufferSize) = 0;
		virtual const RHI_RESULT DestroyBuffer(IDevice* _Device) = 0;

		virtual VulkanBuffer* CastToVulkan() = 0;

		virtual void UpdateUBO(IDevice* _Device, void* _Data, size_t _DataSize) = 0;
	};
}