#pragma once

#include "../VulkanOOP.h"
#include "../Engine/EngineObject.h"

// 윈도우 Surface(vk::raii::SurfaceKHR)를 생성하는 과정
// create() :
//   요구사항 1 : instance, window 참조 검증 및 핸들 가져오기
//   요구사항 2 : glfwCreateWindowSurface로 C 핸들(VkSurfaceKHR) 생성
//   요구사항 3 : C 핸들을 vk::raii::SurfaceKHR로 승격
class VulkanSurface : public EngineObject
{
public:
	VulkanSurface();
	VulkanSurface(const class VulkanInstance* inputInstance, GLFWwindow* inputWindow);
	virtual ~VulkanSurface();

	inline const vk::raii::SurfaceKHR* getSurfaceHandle() const
	{
		return &surface;
	}

private:
	virtual void create() override;

	GLFWwindow* windowRef = nullptr;
	const class VulkanInstance* instanceRef = nullptr;
	vk::raii::SurfaceKHR surface = nullptr;
};