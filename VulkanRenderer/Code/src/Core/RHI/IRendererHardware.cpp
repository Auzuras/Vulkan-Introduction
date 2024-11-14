#include "RHI/IRendererHardware.h"

namespace Core
{
    void IRendererHardware::DestroyDevice(IDevice* _Device)
    {
        _Device->Terminate();

        delete _Device;
        _Device = nullptr;
    }

    void IRendererHardware::DestroySwapChain(ISwapChain* _SwapChain, IDevice* _Device)
    {
        _SwapChain->DestroySwapChain(_Device);

        delete _SwapChain;
        _SwapChain = nullptr;
    }

    void IRendererHardware::DestroyPipeline(IPipeline* _Pipeline, IDevice* _Device)
    {
        _Pipeline->DestroyPipeline(_Device);

        delete _Pipeline;
        _Pipeline = nullptr;
    }

    void IRendererHardware::DestroyCommandAllocator(ICommandAllocator* _CommandAllocator, IDevice* _Device)
    {
        _CommandAllocator->DestroyCommandAllocator(_Device);

        delete _CommandAllocator;
        _CommandAllocator = nullptr;
    }

    void IRendererHardware::DestroyShader(IShader* _Shader)
    {
        delete _Shader;
        _Shader = nullptr;
    }

    void IRendererHardware::DestroyMesh(IMesh* _Mesh)
    {
        delete _Mesh;
        _Mesh = nullptr;
    }

    void IRendererHardware::DestroyTexture(ITexture* _Texture)
    {
        delete _Texture;
        _Texture = nullptr;
    }

    void IRendererHardware::DestroyCommandBuffer(IDevice* _Device, ICommandBuffer* _CommandBuffer)
    {
        delete _CommandBuffer;
        _CommandBuffer = nullptr;
    }

    void IRendererHardware::DestroyCommandBuffers(IDevice* _Device, std::vector<ICommandBuffer*> _CommandBuffers)
    {
        for (ICommandBuffer* commandBuffer : _CommandBuffers)
        {
            delete commandBuffer;
            commandBuffer = nullptr;
        }

        _CommandBuffers.clear();
    }

    void IRendererHardware::DestroyDescriptorAllocator(IDescriptorAllocator* _DescriptorAllocator, IDevice* _Device)
    {
        _DescriptorAllocator->DestroyDescriptorAllocator(_Device);

        delete _DescriptorAllocator;
        _DescriptorAllocator = nullptr;
    }

    void IRendererHardware::DestroySemaphore(ISemaphore* _Semaphore, IDevice* _Device)
    {
        _Semaphore->DestroySemaphoreSync(_Device);

        delete _Semaphore;
        _Semaphore = nullptr;
    }

    void IRendererHardware::DestroyFence(IFence* _Fence, IDevice* _Device)
    {
        _Fence->DestroyFenceSync(_Device);

        delete _Fence;
        _Fence = nullptr;
    }
}