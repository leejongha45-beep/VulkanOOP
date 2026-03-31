#include "Engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::run()
{
	initWindow();

	Initialize();

	Update();

	Release();
}

void Engine::Initialize()
{
}

void Engine::Update()
{
	while (!glfwWindowShouldClose(window))
		glfwPollEvents();
}

void Engine::Release()
{
	glfwDestroyWindow(window);

	glfwTerminate();
}

void Engine::initWindow()
{
	// glfw 초기화
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	constexpr uint32_t WIDTH = 800;
	constexpr uint32_t HEIGHT = 600;

	if (!window)
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}
