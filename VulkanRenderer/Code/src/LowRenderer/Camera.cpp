#include "Camera.h"

namespace LowRenderer
{
    Camera::Camera()
    {}

    void Camera::Update()
    {
        Math::Vector3 look = position.Add(Math::Vector3(0.f, 0.f, 1.f));

        viewMatrix = Math::Matrix4::ViewMatrix(position, look, Math::Vector3::up);
        projectionMatrix = Math::Matrix4::ProjectionPerspectiveMatrix(0.01f, 100.f, 1920.f/1080.f, 45.f);
    }
}