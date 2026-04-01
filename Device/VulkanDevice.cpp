#include "VulkanDevice.h"

#include "../Instance/VulkanInstance.h"

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
	if (!ENSURE(instanceRef)) return;

	auto physicalDevices = instanceRef->getInstance().enumeratePhysicalDevices();
	if (physicalDevices.empty())
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

}

bool VulkanDevice::isDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice)
{
	auto deviceProperties = physicalDevice.getProperties();
	auto deviceFeatures = physicalDevice.getFeatures();

	// eDiscreteGpu : 외장 GPU
	// geometryShader : 지오메트리 셰이더
	if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && deviceFeatures.geometryShader)
		return true;

	return false;
}
