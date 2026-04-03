#include "Engine.h"
#include "../Instance/VulkanInstance.h"
#include "../Device/VulkanPhysicalDevice.h"
#include "../Surface/VulkanSurface.h"
#include "../Device/VulkanDevice.h"
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

	if (!surface)
		surface = new VulkanSurface(dynamic_cast<VulkanInstance*>(instance), window);
	if (ENSURE(surface))
		surface->create();

	if (!physicalDevice)
		physicalDevice = new VulkanPhysicalDevice(dynamic_cast<VulkanInstance*>(instance));
	if (ENSURE(physicalDevice))
		physicalDevice->create();

	if (!device)
		device = new VulkanDevice(dynamic_cast<VulkanPhysicalDevice*>(physicalDevice), dynamic_cast<VulkanSurface*>(surface));
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
	if (device)
	{
		delete device;
		device = nullptr;
	}

	if (physicalDevice)
	{
		delete physicalDevice;
		physicalDevice = nullptr;
	}

	if (surface)
	{
		delete surface;
		surface = nullptr;
	}

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
