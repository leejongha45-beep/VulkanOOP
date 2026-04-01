#pragma once

#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

#define ENSURE(expr) \
    ([&]() -> bool { \
        if (!(expr)) { \
            std::cerr << "[ENSURE FAILED] " << #expr \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            return false; \
        } \
        return true; \
    }())