#pragma once

#include "../VulkanOOP.h"
#include "../Engine/EngineObject.h"

class VulkanInstance : public EngineObject
{
public:
	VulkanInstance();
	virtual ~VulkanInstance();

private:
	vk::raii::Context context;
	vk::raii::Instance instance = nullptr;
	virtual void create() override;
};