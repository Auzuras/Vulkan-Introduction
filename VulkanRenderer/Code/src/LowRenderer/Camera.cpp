#include "Camera.h"
#include "Renderer.h"

namespace LowRenderer
{
    Camera::Camera()
    {}

    void Camera::SetupDescriptors()
    {
        p_UniformBuffers.resize(Core::Renderer::MAX_FRAMES_IN_FLIGHT);
        p_Descriptors.resize(Core::Renderer::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < Core::Renderer::MAX_FRAMES_IN_FLIGHT; ++i)
        {
            p_UniformBuffers[i] = Core::Renderer::GetRHI()->CreateBuffer(Core::Renderer::GetDevice(), Core::BufferType::RHI_UNIFORM_BUFFER, sizeof(CameraData));
        }

        p_Descriptors = Core::Renderer::GetDescriptorAllocator()->CreateUBODescriptor(Core::Renderer::GetDevice(),
            Core::Renderer::MAX_FRAMES_IN_FLIGHT, p_UniformBuffers,
            sizeof(CameraData), Core::Renderer::GetPipeline()->GetDescriptorLayouts()[0]);
    }

    void Camera::DeleteDescriptors()
    {
        for (Core::IBuffer* buffer : p_UniformBuffers)
        {
            Core::Renderer::GetRHI()->DestroyBuffer(buffer, Core::Renderer::GetDevice());
        }

        for (Core::IDescriptor* descriptor : p_Descriptors)
        {
            delete descriptor;
            descriptor = nullptr;
        }
    }

    void Camera::Update()
    {
        Math::Vector3 look = position.Add(Math::Vector3(0.f, 0.f, 1.f));

        viewMatrix = Math::Matrix4::ViewMatrix(position, look, Math::Vector3(0.f, -1.f, 0.f));
        projectionMatrix = Math::Matrix4::ProjectionPerspectiveMatrix(0.01f, 100.f, 1920.f/1080.f, 45.f);
    }

    void Camera::Draw()
    {
    }
}