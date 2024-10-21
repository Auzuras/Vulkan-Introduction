#include "RHI/VulkanWrapper.h"

namespace Core
{
	const bool VulkanWrapper::Initialize()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		return true;
	}

	const bool VulkanWrapper::Terminate()
	{
		return true;
	}
}
