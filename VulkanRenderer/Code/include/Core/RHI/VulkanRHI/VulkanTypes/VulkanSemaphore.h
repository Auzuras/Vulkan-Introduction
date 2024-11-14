#pragma once

#include "RHI/RHITypes/ISemaphore.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"

namespace Core
{
	class VulkanSemaphore : public ISemaphore
	{
	private:
		VkSemaphore m_Semaphore;

	public:
		~VulkanSemaphore() override;

		RHI_RESULT CreateSemaphoreSync(IDevice* _Device) override;
		RHI_RESULT DestroySemaphoreSync(IDevice* _Device) override;

		inline VulkanSemaphore* CastToVulkan() override { return this; }
		inline VkSemaphore& GetSemaphore() { return m_Semaphore; }
	};
}