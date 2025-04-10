#pragma once

#include "Core/Application.hpp"
#include "Graphics/Renderer.hpp"
#include "VulkanCommon.hpp"
#include "VulkanContext.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"

class VulkanBackend : public RendererBackend
{
public:
    explicit VulkanBackend(const ApplicationConfig& config);
    ~VulkanBackend() override;

    auto GetType() const -> RendererAPI override;
    auto GetTypeString() const -> const char* override;
private:
    VulkanContext mContext;
    VulkanDevice mDevice;
    VulkanSwapchain mSwapchain;
};