#include "VulkanDevice.h"

#include "../Instance/VulkanInstance.h"
#include <ranges>

VulkanDevice::VulkanDevice()
{
}

VulkanDevice::VulkanDevice(const VulkanInstance& input)
{
	instanceRef = &input;
	ENSURE(instanceRef);
}

VulkanDevice::~VulkanDevice()
{
}

void VulkanDevice::create()
{
	// instance참조 nullptr확인
	if (!ENSURE(instanceRef))
		return;

	// instanceHandle 가져오기
	const auto* instanceHandle = instanceRef->getInstance();
	if (!ENSURE(instanceHandle)) return;

	// 물리장치 배열 가져오기
	const auto physicalDevices = instanceHandle->enumeratePhysicalDevices();
	if (physicalDevices.empty())
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	// 가능한 물리장치 모두 담기
	auto supportedPhysicalDevices = std::ranges::filter_view(
		physicalDevices,
		[this](const auto pd)
		{
			return isDeviceSuitable(pd);
		});

	if (supportedPhysicalDevices.empty())
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	std::vector supportedDevicesVector(supportedPhysicalDevices.begin(), supportedPhysicalDevices.end());

	uint16_t num = 1;
	for (const auto& view : supportedDevicesVector)
	{
		std::cout << "[" << num++ << "] : " << view.getProperties().deviceName << std::endl;
	}

	bool isNotChoosed = true;
	uint16_t Input	  = 0;
	while (isNotChoosed)
	{
		std::cout << "Choose your PhysicalDevice : ";
		std::cin >> Input;
		if (Input < 1 || Input > static_cast<uint16_t>(supportedDevicesVector.size()))
		{
			std::cout << "Wrong Number";
			continue;
		}
		else
			isNotChoosed = false;
	}

	// 찾은 물리장치 저장
	physicalDevice = supportedDevicesVector[Input - 1];
	ENSURE(*physicalDevice);
}

bool VulkanDevice::isDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice)
{
	// Vulkan APIVersion 지원 확인
	bool supportsVulkan1_3 = physicalDevice.getProperties().apiVersion >= vk::ApiVersion13;

	// graphic queueFamily를 지원하는지 확인
	auto queueFamilies	  = physicalDevice.getQueueFamilyProperties();
	bool supportsGraphics = std::ranges::any_of(
		queueFamilies,
		[](const auto& queueFamily)
		{
			return !!(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics);
		});

	// 필요한 물리장치 확장을 지원하는지 확인
	auto availableDeviceExtensions	   = physicalDevice.enumerateDeviceExtensionProperties();
	bool supportsAllRequiredExtensions = std::ranges::all_of(
		requiredDeviceExtensions,
		[&availableDeviceExtensions](const auto& requiredDeviceExtension)
		{
			return std::ranges::any_of(
				availableDeviceExtensions,
				[requiredDeviceExtension](const auto& availableDeviceExtension)
				{
					return std::strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0;
				});
		});

	// 물리장치가 필요한 기능을 지원하는지 확인
	auto features = physicalDevice.template getFeatures2<
		vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features,
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
	bool supportsRequiredFeatures =
		features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
		features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

	return supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
}
