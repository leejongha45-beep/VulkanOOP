#pragma once

#include "../VulkanOOP.h"
#include "../Engine/EngineObject.h"

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

// vk::raii::Instance를 생성하는 과정
// 요구사항 1 : 앱 정보 생성 (vk::ApplicationInfo appInfo)
// 요구사항 2 : 지원하는 확장 검사 (requiredExtensions, extensionProperties, unsupportedProperties)
// 요구사항 3 : 지원하는 디버깅 레이어 검사 (requiredLayers, layerProperties, unsupportedLayers)
// 요구사항 4 : 실제 인스턴스 정보 생성 (vk::InstanceCreateInfo createInfo)
class VulkanInstance : public EngineObject
{
public:
	VulkanInstance();
	virtual ~VulkanInstance();

	inline const vk::raii::Instance& getInstance() const
	{
		ENSURE(!*instance);
		return instance;
	}

private:
	virtual void create() override;

	vk::raii::Context context;
	vk::raii::Instance instance = nullptr;
	std::vector<const char*> getRequiredInstanceExtensions();
};