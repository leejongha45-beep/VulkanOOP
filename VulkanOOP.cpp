#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Engine/Engine.h"

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

int main()
{
	Engine engine;

	engine.run();

	return 0;
}
