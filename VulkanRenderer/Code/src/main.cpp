#ifndef NDEBUG
#include <crtdbg.h>
#endif

#include "Application.h"

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

	while (!glfwWindowShouldClose(app))
	{
		glfwPollEvents();
		app.Draw();
	}

	if (!app.Terminate())
	{
		return -1;
	}

	return 0;
}