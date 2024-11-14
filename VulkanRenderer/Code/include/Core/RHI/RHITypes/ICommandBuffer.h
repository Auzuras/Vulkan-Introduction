#pragma once

#include "RHI/RHITypes/RHIResult.h"

namespace Core
{
	class IDevice;
	class ICommandAllocator;
	class IMesh;

	class ICommandBuffer
	{
	public:
		virtual void ResetCommandBuffer() = 0;
		virtual const RHI_RESULT StartRecordingCommandBuffer() = 0;
		virtual const RHI_RESULT StopRecordingCommandBuffer() const = 0;

		virtual void BindVertexBuffer(IMesh* _Mesh) const = 0;
		virtual void BindIndexBuffer(IMesh* _Mesh) const = 0;
		virtual void DrawIndexed(IMesh* _Mesh) const = 0;
		virtual void EndRenderPass() const = 0;
	};
}