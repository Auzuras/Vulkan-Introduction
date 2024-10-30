#ifndef NDEBUG
#include <crtdbg.h>
#endif

#include "Application.h"
#include "Object.h"

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
	//_CrtSetBreakAlloc(1356);
#endif
	Core::Application application(800, 600, "Vulkan Renderer");

	if (!application.Initialize())
	{
		return -1;
	}
	
	LowRenderer::Object test;
	test.transform.GetPosition().Print();

	while (!glfwWindowShouldClose(application.GetWindow()))
	{
		glfwPollEvents();
		application.Draw();
	}

	if (!application.Terminate())
	{
		return -1;
	}

	return 0;
}