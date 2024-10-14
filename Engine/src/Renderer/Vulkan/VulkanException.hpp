#pragma once

#include <vulkan/vulkan.h>

class VulkanException : public NihilException
{
public:
    VulkanException(const char* file, u32 line, std::string_view message, VkResult result);
private:
    const char* VkResultToDescription(VkResult result);
};

#define VK_THROW(message, result) throw VulkanException(__FILE__, __LINE__, message, result)