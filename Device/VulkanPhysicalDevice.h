#pragma once

#include "../VulkanOOP.h"
#include "../Engine/EngineObject.h"

// 물리장치를 선택하고 저장하는 과정
// create() :
//   요구사항 1 : instance 참조 검증 (instanceRef)
//   요구사항 2 : 물리장치 배열 가져오기 (physicalDevices)
//   요구사항 3 : 적합한 장치 필터링 및 사용자 선택 (supportedDevicesVector, Input)
//   요구사항 4 : 선택한 물리장치 저장 (physicalDevice)
// isDeviceSuitable() :
//   요구사항 1 : Vulkan API 1.3 이상 지원 확인 (supportsVulkan1_3)
//   요구사항 2 : Graphics Queue Family 지원 확인 (supportsGraphics)
//   요구사항 3 : 필요한 물리장치 확장 지원 확인 (supportsAllRequiredExtensions)
//   요구사항 4 : 필요한 기능 지원 확인 — dynamicRendering, extendedDynamicState (supportsRequiredFeatures)
class VulkanPhysicalDevice : public EngineObject
{
public:
	VulkanPhysicalDevice();
	VulkanPhysicalDevice(const class VulkanInstance& input);
	virtual ~VulkanPhysicalDevice();

	inline const vk::raii::PhysicalDevice* getPhysicalDevice() const
	{
		return &physicalDevice;
	}

	inline const std::vector<const char*>* getRequiredDeviceExtensions() const
	{
		return &requiredDeviceExtensions;
	}

private:
	virtual void create() override;

	vk::raii::PhysicalDevice physicalDevice = nullptr;
	const class VulkanInstance* instanceRef = nullptr;
	std::vector<const char*> requiredDeviceExtensions = { vk::KHRSwapchainExtensionName };
	bool isDeviceSuitable(const vk::raii::PhysicalDevice& physicalDeivce);
};