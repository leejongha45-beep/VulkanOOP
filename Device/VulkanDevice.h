#pragma once

#include "../Engine/EngineObject.h"
#include "../VulkanOOP.h"

// 논리장치(vk::raii::Device)를 생성하는 과정
// create() :
//   요구사항 1 : physicalDevice, surface 참조 검증 및 핸들 가져오기
//   요구사항 2 : 그래픽+프레젠트 동시 지원 큐 패밀리 인덱스 찾기 (queueIndex)
//   요구사항 3 : 기능 체이닝 설정 — dynamicRendering, extendedDynamicState (featureChain)
//   요구사항 4 : 논리장치 생성 (device, graphicsQueue)
class VulkanDevice : public EngineObject
{
public:
	VulkanDevice();
	VulkanDevice(const class VulkanPhysicalDevice* inputPhysicalDevice, const class VulkanSurface* inputSurface);
	virtual ~VulkanDevice();

private:
	virtual void create() override;

	vk::raii::Device device = nullptr;
	const class VulkanPhysicalDevice* physicalDeviceRef = nullptr;
	const class VulkanSurface* surfaceRef = nullptr;
	vk::raii::Queue graphicsQueue = nullptr;
};