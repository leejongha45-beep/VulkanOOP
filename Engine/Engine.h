#pragma once

#include "../VulkanOOP.h"

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

	class EngineObject* instance = nullptr;
};