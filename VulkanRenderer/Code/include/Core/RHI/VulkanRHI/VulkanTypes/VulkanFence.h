#pragma once

#include "RHI/RHITypes/IFence.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanFence : public IFence
	{
	private:
		VkFence m_Fence;

	public:
		~VulkanFence() override;

		RHI_RESULT CreateFenceSync(IDevice* _Device) override;
		RHI_RESULT DestroyFenceSync(IDevice* _Device) override;
	};
}