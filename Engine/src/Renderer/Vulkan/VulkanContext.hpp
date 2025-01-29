#pragma once

#include "Core/Application.hpp"
#include "VulkanCommon.hpp"

class VulkanContext
{
public:
    explicit VulkanContext(const ApplicationConfig& appConfig);
    ~VulkanContext();
public:
    VkSurfaceKHR Surface {};
    VkInstance Instance {};
#ifndef NDEBUG
private:
    VkDebugUtilsMessengerEXT mDebugMessenger {};
#endif
};