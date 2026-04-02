#include "Engine.h"
#include "../Device/VulkanDevice.h"
#include "../Instance/VulkanInstance.h"
#include "EngineObject.h"

Engine::Engine()
{
}

Engine::~Engine()
{
	Release();
}

void Engine::run()
{
	initWindow();

	Initialize();

	Update();
}

void Engine::Initialize()
{
	if (!instance)
		instance = new VulkanInstance;
	if (ENSURE(instance))
		instance->create();

	if (!device)
		device = new VulkanDevice(*dynamic_cast<VulkanInstance*>(instance));
	if (ENSURE(device))
		device->create();
}

void Engine::Update()
{
	while (!glfwWindowShouldClose(window))
		glfwPollEvents();
}

void Engine::Release()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}

	if (window)
	{
		glfwDestroyWindow(window);
		window = nullptr;
	}

	glfwTerminate();
}

void Engine::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	constexpr uint32_t WIDTH  = 800;
	constexpr uint32_t HEIGHT = 600;

	if (!window)
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	ENSURE(window);
}
