#include "VulkanPlugin.hpp"

void VulkanPlugin::Initialize()
{
    LOG_INFO("HELLO FROM VULKAN PLUGIN");
}

VulkanPlugin::~VulkanPlugin()
{
    LOG_INFO("HELLO FROM VULKAN PLUGIN DESTROYED");
}

RendererPlugin* CreatePlugin()
{
    return new VulkanPlugin();
}

void DestroyPlugin(RendererPlugin* plugin)
{
    delete plugin;
}