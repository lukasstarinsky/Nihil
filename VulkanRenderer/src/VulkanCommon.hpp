#pragma once

#include <string>
#include <Nihil.hpp>
#include <vulkan/vulkan.h>
#include "VulkanTypes.hpp"

#define VK_CHECK(fun) do { VkResult result { fun }; NTHROW_IF(result != VK_SUCCESS, "Vulkan call failed with error code: " + std::to_string(result)); } while(0)