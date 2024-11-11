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

    void IRendererHardware::DestroyShader(IShader* _Shader, IDevice* _Device)
    {
        delete _Shader;
        _Shader = nullptr;
    }
}