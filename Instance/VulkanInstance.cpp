#include "VulkanInstance.h"

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
}

void VulkanInstance::create()
{
	constexpr vk::ApplicationInfo appInfo
	{
		.pApplicationName = "Hello Triangle", // 앱 이름
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0), // 앱 버전
		.pEngineName = "No Engine", // 엔진 이름
		.engineVersion = VK_MAKE_VERSION(1, 0, 0), // 엔진 버전
		.apiVersion = vk::ApiVersion14 // 사용할 Vulkan API 버전
	};

	uint32_t requiredExtensionCount = 0;
	auto requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);

	auto extensionProperties = context.enumerateInstanceExtensionProperties();
	for (uint32_t i = 0; i < requiredExtensionCount; ++i)
	{
		if (std::ranges::none_of(extensionProperties, [requiredExtension = requiredExtensions[i]](const auto& extensionProperty)
			{
				return std::strcmp(extensionProperty.extensionName, requiredExtension) == 0;
			}))
		{
			throw std::runtime_error("Required extension not supported");
		}
	}

	vk::InstanceCreateInfo createInfo
	{
		.pApplicationInfo = &appInfo, // 앱 메타데이터 포인터
		.enabledExtensionCount = requiredExtensionCount, // 활성화할 인스턴스 확장 개수
		.ppEnabledExtensionNames = requiredExtensions // 확장 이름 문자열 배열
	};

	instance = vk::raii::Instance(context, createInfo);
}