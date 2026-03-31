#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Engine
{
public:
	Engine();
	~Engine();

	void run();

private:
	void Initialize();
	void Update();
	void Release();

private:
	GLFWwindow* window = nullptr;

	// 창 생성
	void initWindow();
	
};