#pragma once

#include "RHI/RHITypes/IQueue.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"


namespace Core
{
	class VulkanDevice;

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transfertFamily;

		bool isComplete = false;
	};

	class VulkanQueue : public IQueue
	{
	private:
		VkQueue m_Queue;

	public:

		// TODO: Check if necessary
		RHI_RESULT CreateQueue();
		RHI_RESULT DestroyQueue();

		inline VkQueue& GetQueue() { return m_Queue; }

		/// <summary>
		/// Find all queue famillies suporting features required by the application (Graphics and presentation)
		/// </summary>
		/// <param name="_Device">: Physical device containing queue families </param>
		/// <returns></returns>
		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _Device, VkSurfaceKHR _Surface);
	};
}