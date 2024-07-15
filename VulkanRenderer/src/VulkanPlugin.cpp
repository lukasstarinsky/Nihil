#include <vulkan/vulkan_core.h>
#include "VulkanPlugin.hpp"

struct VulkanState
{
    VkInstance Instance;
    VkPhysicalDevice PhysicalDevice;
    VkDebugUtilsMessengerEXT DebugMessenger;
};

static VulkanState sState;

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        LOG_FATAL("%s", callbackData->pMessage);
    }

    return VK_FALSE;
}

VulkanPlugin::VulkanPlugin()
{
    LOG_TRACE("Initializing Vulkan renderer...");

#ifdef NDEBUG
    bool enableValidation { false };
#else
    bool enableValidation { true };
#endif

    /* ======= Instance ======= */
    VkApplicationInfo applicationInfo {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "Nihil Application";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "Nihil";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    const char* enabledExtensions[] { VK_KHR_SURFACE_EXTENSION_NAME,
#ifndef NDEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
#ifdef NIHIL_PLATFORM_WINDOWS
        "VK_KHR_win32_surface"
#endif
    };

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = COUNT_OF(enabledExtensions);
    createInfo.ppEnabledExtensionNames = enabledExtensions;
    if (enableValidation)
    {
        u32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        const char* validationLayers[] { "VK_LAYER_KHRONOS_validation" };
        bool validationSupported {};
        for (const auto& layerProperties: availableLayers)
        {
            if (!strcmp(validationLayers[0], layerProperties.layerName))
            {
                validationSupported = true;
                break;
            }
        }
        if (!validationSupported)
        {
            NTHROW("Vulkan validation layers are not available.");
        }
        createInfo.enabledLayerCount = COUNT_OF(validationLayers);
        createInfo.ppEnabledLayerNames = validationLayers;
    }

    if (vkCreateInstance(&createInfo, nullptr, &sState.Instance) != VK_SUCCESS)
    {
        NTHROW("Failed to create Vulkan instance.");
    }

    if (enableValidation)
    {
        VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
        messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        messengerCreateInfo.pfnUserCallback = VulkanDebugCallback;

        auto vkCreateDebugUtilsMessengerEXT{ reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(sState.Instance, "vkCreateDebugUtilsMessengerEXT"))};
        if (vkCreateDebugUtilsMessengerEXT(sState.Instance, &messengerCreateInfo, nullptr, &sState.DebugMessenger) != VK_SUCCESS)
        {
            NTHROW("Failed to setup Vulkan debug messenger.");
        }
    }

    /* ======= Device ======= */
    u32 deviceCount {};
    vkEnumeratePhysicalDevices(sState.Instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        NTHROW("Failed to find GPU with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(sState.Instance, &deviceCount, devices.data());
    sState.PhysicalDevice = devices[0];

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(sState.PhysicalDevice, &deviceProperties);
    LOG_INFO("Starting with GPU: %s", deviceProperties.deviceName);
}

VulkanPlugin::~VulkanPlugin()
{
    LOG_TRACE("Shutting down Vulkan renderer...");
#ifndef NDEBUG
    auto vkDestroyDebugUtilsMessengerEXT { reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(sState.Instance, "vkDestroyDebugUtilsMessengerEXT")) };
    vkDestroyDebugUtilsMessengerEXT(sState.Instance, sState.DebugMessenger, nullptr);
#endif

    vkDestroyInstance(sState.Instance, nullptr);
}

RendererPlugin* CreatePlugin()
{
    return new VulkanPlugin();
}

void DestroyPlugin(RendererPlugin* plugin)
{
    delete plugin;
}