#ifndef NDEBUG
#include <crtdbg.h>
#endif

#include "Application.h"

#include "Time/Time.h"
#include "Renderer.h"
#include "Maths/Utils/Utils.h"

// Selects GPU
extern "C"
{
	_declspec(dllexport) uint32_t NvOptimusEnablement = 1;
	_declspec(dllexport) int32_t AmdPowerXpressRequestHighPerformance = 1;
}

int main()
{
	// Checks memory leaks
#ifndef NDEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(64719);
#endif

	Core::Application app;

	if (!app.Initialize())
	{
		return -1;
	}

	float fRot = 0.f;

	while (!app.GetWindow()->WindowShouldClose())
	{
		app.GetWindow()->WindowPollEvents();
		app.appCamera.Update();
		
		Core::Renderer::model.m_Transform.m_LocalTRS = Math::Matrix4::TRS(Math::Vector3(1.f, 1.f, 0.f), Math::Vector3(Math::Utils::DegToRad(-90.f), Math::Utils::DegToRad(0.f), Math::Utils::DegToRad(-90.f)), Math::Vector3::one).Transpose();
		Core::Renderer::mcModel.m_Transform.m_LocalTRS = Math::Matrix4::TRS(Math::Vector3(-1.f, 0.f, 0.f), Math::Vector3(0.f, fRot += 1.f * Core::Time::deltaTime, 0.f), Math::Vector3::one).Transpose();

		app.Draw();

		if (glfwGetKey(app.GetWindow()->GetWindowPointer(), GLFW_KEY_W) == GLFW_PRESS)
		{
			app.appCamera.position = app.appCamera.position.Add(Math::Vector3(0.f, 0.f, 1.f) * Core::Time::deltaTime * 2.f);
		}

		if (glfwGetKey(app.GetWindow()->GetWindowPointer(), GLFW_KEY_S) == GLFW_PRESS)
		{
			app.appCamera.position = app.appCamera.position.Add(Math::Vector3(0.f, 0.f, -1.f) * Core::Time::deltaTime * 2.f);
		}

		if (glfwGetKey(app.GetWindow()->GetWindowPointer(), GLFW_KEY_D) == GLFW_PRESS)
		{
			app.appCamera.position = app.appCamera.position.Add(Math::Vector3(1.f, 0.f, 0.f) * Core::Time::deltaTime * 2.f);
		}

		if (glfwGetKey(app.GetWindow()->GetWindowPointer(), GLFW_KEY_A) == GLFW_PRESS)
		{
			app.appCamera.position = app.appCamera.position.Add(Math::Vector3(-1.f, 0.f, 0.f) * Core::Time::deltaTime * 2.f);
		}

		if (glfwGetKey(app.GetWindow()->GetWindowPointer(), GLFW_KEY_Q) == GLFW_PRESS)
		{
			app.appCamera.position = app.appCamera.position.Add(Math::Vector3(0.f, 1.f, 0.f) * Core::Time::deltaTime * 2.f);
		}

		if (glfwGetKey(app.GetWindow()->GetWindowPointer(), GLFW_KEY_E) == GLFW_PRESS)
		{
			app.appCamera.position = app.appCamera.position.Add(Math::Vector3(0.f, -1.f, 0.f) * Core::Time::deltaTime * 2.f);
		}

		float fps = 1.f / Core::Time::deltaTime;
		std::string name = "Vulkan Renderer: " + std::to_string(fps);

		app.GetWindow()->SetWindowName(name.c_str());

		// Last thing to execute
		Core::Time::UpdateDeltaTime(static_cast<float>(glfwGetTime()));
	}

	if (!app.Terminate())
	{
		return -1;
	}

	return 0;
}