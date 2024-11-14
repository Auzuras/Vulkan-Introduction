#pragma once

#include "RHI/RHITypes/RHIResult.h"
#include "Vectors/Vector4.h"
#include "Vectors/Vector2.h"

namespace Core
{
	class IDevice;
	class ICommandAllocator;
	class IMesh;
	class ISwapChain;
	class IPipeline;
	class VulkanCommandBuffer;

	class ICommandBuffer
	{
	public:
		virtual void ResetCommandBuffer() = 0;
		virtual const RHI_RESULT StartRecordingCommandBuffer() = 0;
		virtual const RHI_RESULT StopRecordingCommandBuffer() const = 0;

		virtual VulkanCommandBuffer* CastToVulkan() = 0;

		virtual void StartRenderPass(IPipeline* _Pipeline, ISwapChain* _Swapchain, unsigned int _ImageIndex, Math::Vector4 _ClearColor) const = 0;
		virtual void BindPipeline(IPipeline* _Pipeline) const = 0;

		virtual void SetViewport(Math::Vector2 _Position, ISwapChain* _Swapchain, float _MinDepth, float _MaxDepth) const = 0;
		virtual void SetScissor(Math::Vector2 _Offset, ISwapChain* _Swapchain) const = 0;

		virtual void BindVertexBuffer(IMesh* _Mesh) const = 0;
		virtual void BindIndexBuffer(IMesh* _Mesh) const = 0;
		virtual void DrawIndexed(IMesh* _Mesh) const = 0;
		virtual void EndRenderPass() const = 0;
	};
}