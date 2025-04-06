#pragma once

#include <vulkan/vulkan.h>

#include "Core/Exception.hpp"

class OpenGLException : public NihilException
{
public:
    explicit OpenGLException(VkResult result);
private:
    static auto VkResultToDescription(VkResult result) -> const char*;
};

#define VK_THROW(result) throw VulkanException(result)