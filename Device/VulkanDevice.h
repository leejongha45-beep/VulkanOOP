#pragma once

#include "../Engine/EngineObject.h"
#include "../VulkanOOP.h"

class VulkanDevice : public EngineObject
{
public:
	VulkanDevice();
	VulkanDevice(const class VulkanPhysicalDevice& input);
	~VulkanDevice();

private:
	virtual void create() override;
	vk::raii::Device device = nullptr;
	const class VulkanPhysicalDevice* physicalDeviceRef = nullptr;
};