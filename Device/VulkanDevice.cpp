#include "VulkanDevice.h"
#include "../Surface/VulkanSurface.h"
#include "VulkanPhysicalDevice.h"

VulkanDevice::VulkanDevice()
{
}

VulkanDevice::VulkanDevice(const VulkanPhysicalDevice* input, const VulkanSurface* inputSurface)
{
	physicalDeviceRef = input;
	ENSURE(physicalDeviceRef);

	surfaceRef = inputSurface;
	ENSURE(surfaceRef);
}

VulkanDevice::~VulkanDevice()
{
}

void VulkanDevice::create()
{
	// 요구사항 1 : physicalDevice, surface 참조 검증 및 핸들 가져오기
	if (!ENSURE(physicalDeviceRef))
		return;
	if (!ENSURE(surfaceRef))
		return;
	const auto* physicalDeviceHandle = physicalDeviceRef->getPhysicalDevice();
	if (!ENSURE(physicalDeviceHandle))
		return;
	const auto* surfaceHandle = surfaceRef->getSurfaceHandle();
	if (!ENSURE(surfaceHandle))
		return;

	// 요구사항 2 : 그래픽+프레젠트 동시 지원 큐 패밀리 인덱스 찾기
	// Vulkan API가 큐 패밀리를 이름이 아닌 인덱스(몇번째 칸?)으로 판별하므로 인덱스 자체가 큐 패밀리의 ID
	// 따라서 인덱스를 찾아야함
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDeviceHandle->getQueueFamilyProperties();
	uint32_t queueIndex											 = ~0;
	for (uint32_t qfpIndex = 0; qfpIndex < static_cast<uint32_t>(queueFamilyProperties.size()); qfpIndex++)
	{
		if ((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
			physicalDeviceHandle->getSurfaceSupportKHR(qfpIndex, *surfaceHandle))
		{
			queueIndex = qfpIndex;
			break;
		}
	}

    if (queueIndex == ~0)
	{
		throw std::runtime_error("Could not find a queue for graphics and present -> terminating");
	}

	// 요구사항 3 : 기능 체이닝 설정
	// PhysicalDeviceFeatures2가 시작점이므로 실제기능들은 PhysicalDeviceVulkan13Features,
	// PhysicalDeviceExtendedDynamicStateFeaturesEXT임 (체이닝 구조)
	// PhysicalDeviceFeatures2는 아무 기능없이 진입점 역할
	vk::StructureChain<
		vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features,
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
		featureChain = {{}, {.dynamicRendering = true}, {.extendedDynamicState = true}};

	const auto* requiredDeviceExtension = physicalDeviceRef->getRequiredDeviceExtensions();
	if (!ENSURE(requiredDeviceExtension))
		return;

	// 요구사항 4 : 논리장치 생성
	float queuePriority = 0.5f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo{
		.queueFamilyIndex = queueIndex, .queueCount = 1, .pQueuePriorities = &queuePriority};

	vk::DeviceCreateInfo deviceCreateInfo{
		.pNext					 = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
		.queueCreateInfoCount	 = 1,
		.pQueueCreateInfos		 = &deviceQueueCreateInfo,
		.enabledExtensionCount	 = static_cast<uint32_t>(requiredDeviceExtension->size()),
		.ppEnabledExtensionNames = requiredDeviceExtension->data()};

	device		  = vk::raii::Device(*physicalDeviceHandle, deviceCreateInfo);
	graphicsQueue = vk::raii::Queue(device, queueIndex, 0);
}
