#pragma once

#include "Core/Application.hpp"
#include "VulkanCommon.hpp"

class VulkanContext
{
public:
    explicit VulkanContext(const ApplicationConfig& appConfig);
    ~VulkanContext();

    auto GetSurface() const -> VkSurfaceKHR;
    auto GetInstance() const -> VkInstance;
private:
    VkSurfaceKHR mSurface {};
    VkInstance mInstance {};
#ifndef NDEBUG
    VkDebugUtilsMessengerEXT mDebugMessenger {};
#endif
};