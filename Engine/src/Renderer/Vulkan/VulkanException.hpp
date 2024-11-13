#pragma once

#include <vulkan/vulkan.h>

class VulkanException : public NihilException
{
public:
    explicit VulkanException(VkResult result);
private:
    static auto VkResultToDescription(VkResult result) -> const char*;
};

#define VK_THROW(result) throw VulkanException(result)