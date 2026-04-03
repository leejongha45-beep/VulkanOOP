#include "VulkanSurface.h"
#include "../Instance/VulkanInstance.h"

VulkanSurface::VulkanSurface()
{
}

VulkanSurface::VulkanSurface(const VulkanInstance* inputInstance, GLFWwindow* inputWindow)
{
	instanceRef = inputInstance;
	ENSURE(instanceRef);

	windowRef	= inputWindow;
	ENSURE(windowRef);
}

VulkanSurface::~VulkanSurface()
{
}

void VulkanSurface::create()
{
	// 요구사항 1 : instance, window 참조 검증 및 핸들 가져오기
	if (!ENSURE(instanceRef))
		return;
	if (!ENSURE(windowRef))
		return;

	const auto* instanceHandle = instanceRef->getInstanceHandle();
	if (!ENSURE(instanceHandle))
		return;

	// 요구사항 2 : glfwCreateWindowSurface로 C 핸들(VkSurfaceKHR) 생성
	VkSurfaceKHR _surface;
	if (glfwCreateWindowSurface(**instanceHandle, windowRef, nullptr, &_surface) != 0)
	{
		throw std::runtime_error("failed to create window surface!");
	}

	// 요구사항 3 : C 핸들을 vk::raii::SurfaceKHR로 승격
	surface = vk::raii::SurfaceKHR(*instanceHandle, _surface);
}
