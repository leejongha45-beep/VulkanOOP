#include "VulkanInstance.h"
#include <ranges>

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
}

// vk::raii::Instance를 생성하는 과정
void VulkanInstance::create()
{
	// 요구사항 1 : 앱 정보 생성
	constexpr vk::ApplicationInfo appInfo
	{
		.pApplicationName = "Hello Triangle", // 앱 이름
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0), // 앱 버전
		.pEngineName = "No Engine", // 엔진 이름
		.engineVersion = VK_MAKE_VERSION(1, 0, 0), // 엔진 버전
		.apiVersion = vk::ApiVersion14 // 사용할 Vulkan API 버전
	};

	// 요구사항 2 : 지원하는 확장 검사
	auto requiredExtensions = getRequiredInstanceExtensions(); // 필요한 확장목록
	auto extensionProperties = context.enumerateInstanceExtensionProperties(); // 현재 지원되는 확장 목록
	auto unsupportedProperties = std::ranges::filter_view(requiredExtensions, [&extensionProperties](const auto& requiredProperty)
		{
			return std::ranges::none_of(extensionProperties, [requiredProperty](const auto& extensionProperty)
				{
					return std::strcmp(extensionProperty.extensionName, requiredProperty) == 0;
				});
		});

	for (const auto& err : unsupportedProperties)
		std::cerr << "[Unsupported] " << err << std::endl;

	if (!std::ranges::empty(unsupportedProperties))
		throw std::runtime_error("Required extensions not supported");

	// 요구사항 3 : 지원하는 디버깅 레이어 검사
	std::vector<const char*> requiredLayers;
	if (enableValidationLayers)
	{
		requiredLayers.assign(validationLayers.begin(), validationLayers.end());
	}

	auto layerProperties = context.enumerateInstanceLayerProperties(); // 현재 지원되는 레이어 목록
	auto unsupportedLayers = std::ranges::filter_view(requiredLayers, [&layerProperties](const auto& requiredLayer)
		{
			return std::ranges::none_of(layerProperties, [requiredLayer](const auto& layerProperty)
				{
					return std::strcmp(layerProperty.layerName, requiredLayer) == 0;
				});
		});

	for (const auto& err : unsupportedLayers)
		std::cerr << "[Unsupported] " << err << std::endl;

	if (!std::ranges::empty(unsupportedLayers))
		throw std::runtime_error("Required layers not supported");

	// 요구사항 4 : 실제 인스턴스 정보 생성
	vk::InstanceCreateInfo createInfo
	{
		.pApplicationInfo = &appInfo, // 앱 메타데이터 포인터
		.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()), // 활성화할 인스턴스 확장 개수
		.ppEnabledExtensionNames = requiredExtensions.data() // 확장 이름 문자열 배열
	};

	// 마지막 실제 메모리 잡기
	instance = vk::raii::Instance(context, createInfo);
}

std::vector<const char*> VulkanInstance::getRequiredInstanceExtensions()
{
	uint32_t glfwExtensionCount = 0;
	auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return extensions;
}
