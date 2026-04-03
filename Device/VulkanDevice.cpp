#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"

VulkanDevice::VulkanDevice()
{
}

VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& input)
{
	physicalDeviceRef = &input;
	ENSURE(physicalDeviceRef);
}

VulkanDevice::~VulkanDevice()
{
}

void VulkanDevice::create()
{
	if (!ENSURE(physicalDeviceRef))
		return;

	auto* physicalDeviceHandle = physicalDeviceRef->getPhysicalDevice();
	if (!ENSURE(physicalDeviceHandle))
		return;

	// 그래픽 큐 패밀리 찾기
	// Vulkan API가 큐 패밀리를 이름이 아닌 인덱스(몇번째 칸?)으로 판별하므로 인덱스 자체가 큐 패밀리의 ID 그래서
	// 인덱스를 찾아야함
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDeviceHandle->getQueueFamilyProperties();
	auto graphicsQueueFamilyProperty							 = std::ranges::find_if(
		queueFamilyProperties,
		[](const auto& qfp)
		{
			return (qfp.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0);
		});
	auto graphicsIndex =
		static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));

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

	// 장치 생성
	float queuePriority = 0.5f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo{
		.queueFamilyIndex = graphicsIndex, .queueCount = 1, .pQueuePriorities = &queuePriority};

	vk::DeviceCreateInfo deviceCreateInfo{
		.pNext					 = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
		.queueCreateInfoCount	 = 1,
		.pQueueCreateInfos		 = &deviceQueueCreateInfo,
		.enabledExtensionCount	 = static_cast<uint32_t>(requiredDeviceExtension->size()),
		.ppEnabledExtensionNames = requiredDeviceExtension->data()};

	device = vk::raii::Device(*physicalDeviceHandle, deviceCreateInfo);
}
