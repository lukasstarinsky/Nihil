#include "VulkanPlatform.hpp"
#include "VulkanContext.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, [[maybe_unused]] void* userData)
{
    if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        return VK_FALSE;

    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        {
            Logger::Info("{}", callbackData->pMessage);
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        {
            Logger::Warn("{}", callbackData->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        {
            Logger::Error("{}", callbackData->pMessage);
            break;
        }
    }

    return VK_FALSE;
}


VulkanContext::VulkanContext(const ApplicationConfig& appConfig)
{
    const char* instanceExtensions[] {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VulkanPlatform::GetSurfaceExtension(),
#ifndef NDEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
    };

    VkApplicationInfo appInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = appConfig.Name.c_str(),
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "Nihil",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3
    };

    VkInstanceCreateInfo instanceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = COUNT_OF(instanceExtensions),
        .ppEnabledExtensionNames = instanceExtensions
    };
#ifndef NDEBUG
    const char* instanceLayers[] {
        "VK_LAYER_KHRONOS_validation"
    };

    u32 layerCount;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

    std::vector<VkLayerProperties> availableLayers(layerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));

    bool validationSupported = false;
    for (const auto& layerProperties: availableLayers)
    {
        if (!strcmp(instanceLayers[0], layerProperties.layerName))
        {
            validationSupported = true;
            break;
        }
    }
    Ensure(validationSupported, "Vulkan validation layers are not available.");

    instanceCreateInfo.enabledLayerCount = COUNT_OF(instanceLayers);
    instanceCreateInfo.ppEnabledLayerNames = instanceLayers;
#endif

    VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance));

#ifndef NDEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = VulkanDebugCallback,
        .pUserData = nullptr
    };

    auto vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT"));
    VK_CHECK(vkCreateDebugUtilsMessengerEXT(mInstance, &debugMessengerCreateInfo, nullptr, &mDebugMessenger));
#endif

    mSurface = VulkanPlatform::CreateSurface(mInstance);
}

VulkanContext::~VulkanContext()
{
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
#ifndef NDEBUG
    auto vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT"));
    vkDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif
    vkDestroyInstance(mInstance, nullptr);
}

auto VulkanContext::GetSurface() const -> VkSurfaceKHR
{
    return mSurface;
}

auto VulkanContext::GetInstance() const -> VkInstance
{
    return mInstance;
}