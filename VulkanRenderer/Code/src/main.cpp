#ifndef NDEBUG
#include <crtdbg.h>
#endif

#include "Application/Application.h"

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
	//_CrtSetBreakAlloc(991);
#endif

	Core::Application application(800, 600, "Vulkan Renderer", Core::RHIType::VULKAN);

	if (!application.Initialize())
	{
		return -1;
	}

	while (!glfwWindowShouldClose(application.GetWindow()))
	{
		glfwPollEvents();
	}

	if (!application.Terminate())
	{
		return -1;
	}

	return 0;
}