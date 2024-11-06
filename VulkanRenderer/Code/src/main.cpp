#ifndef NDEBUG
#include <crtdbg.h>
#endif

#include "Application.h"

#include "Time/Time.h"

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
	//_CrtSetBreakAlloc(1962);
#endif

	Core::Application app;

	if (!app.Initialize())
	{
		return -1;
	}

	while (!glfwWindowShouldClose(app.GetWindow()->GetWindowPointer()))
	{
		glfwPollEvents();
		app.Draw();
		app.appCamera.Update();

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

		// Last thing to execute
		Core::Time::UpdateDeltaTime(static_cast<float>(glfwGetTime()));
	}

	if (!app.Terminate())
	{
		return -1;
	}

	return 0;
}