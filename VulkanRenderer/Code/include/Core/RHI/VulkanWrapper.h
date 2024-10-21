#pragma once
#include <vulkan/vulkan.h>

#include "RHI/RHI.h"

namespace Core
{
	class VulkanWrapper : public RHI
	{
	private:
	public:
		VulkanWrapper() = default;
		~VulkanWrapper() = default;

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		const bool Initialize() override;
		const bool Terminate() override;
	};
}