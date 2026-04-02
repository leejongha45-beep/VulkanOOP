#pragma once

#include "../VulkanOOP.h"
#include "../Engine/EngineObject.h"

class VulkanDevice : public EngineObject
{
public:
	VulkanDevice();
	VulkanDevice(const class VulkanInstance& input);
	virtual ~VulkanDevice();

private:
	virtual void create() override;

	vk::raii::PhysicalDevice physicalDevice = nullptr;
	const class VulkanInstance* instanceRef = nullptr;
	std::vector<const char*> requiredDeviceExtensions = { vk::KHRSwapchainExtensionName };
	bool isDeviceSuitable(const vk::raii::PhysicalDevice& physicalDeivce);
};