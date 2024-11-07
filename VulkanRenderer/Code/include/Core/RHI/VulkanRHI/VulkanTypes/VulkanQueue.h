#pragma once

#include "RHI/RHITypes/IQueue.h"

#include "RHI/VulkanRHI/VulkanRenderer.h"


namespace Core
{
	class VulkanDevice;

	class VulkanQueue : public IQueue
	{
	private:
		VkQueue m_Queue;

	public:
		/// <summary>
		/// Find all queue famillies suporting features required by the application (Graphics and presentation)
		/// </summary>
		/// <param name="_Device">: Physical device containing queue families </param>
		/// <returns></returns>
		static QueueFamilyIndices FindQueueFamilies(VulkanDevice _Device);
	};
}