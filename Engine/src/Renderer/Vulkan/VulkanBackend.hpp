#pragma once

#include "Core/Application.hpp"
#include "Renderer/Renderer.hpp"
#include "VulkanCommon.hpp"
#include "VulkanContext.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"

class VulkanBackend : public RendererBackend
{
public:
    explicit VulkanBackend(const ApplicationConfig& config);
    ~VulkanBackend() override;
private:
    VulkanContext mContext;
    VulkanDevice mDevice;
    VulkanSwapchain mSwapchain;
};