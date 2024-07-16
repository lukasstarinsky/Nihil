#include <unordered_set>
#include "VulkanPlugin.hpp"
#include "Platform/VulkanPlatform.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
//    if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
//        return VK_FALSE;

    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        {
            LOG_INFO("{}", callbackData->pMessage);
            break;
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        {
            LOG_WARN("{}", callbackData->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        {
            LOG_ERROR("{}", callbackData->pMessage);
            break;
        }
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
        .pApplicationName = "Nihil Application",
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
    if (enableValidation)
    {
        const char* instanceLayers[] {
            "VK_LAYER_KHRONOS_validation"
        };

        u32 layerCount;
        VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

        std::vector<VkLayerProperties> availableLayers(layerCount);
        VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));

        bool validationSupported { false };
        for (const auto& layerProperties: availableLayers)
        {
            if (!strcmp(instanceLayers[0], layerProperties.layerName))
            {
                validationSupported = true;
                break;
            }
        }
        NTHROW_IF(!validationSupported, "Vulkan validation layers are not available.");

        instanceCreateInfo.enabledLayerCount = COUNT_OF(instanceLayers);
        instanceCreateInfo.ppEnabledLayerNames = instanceLayers;
    }

    VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance));

    /* ======= Debug Messenger ======= */
    if (enableValidation)
    {
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = VulkanDebugCallback,
            .pUserData = nullptr
        };

        auto vkCreateDebugUtilsMessengerEXT { reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT")) };
        VK_CHECK(vkCreateDebugUtilsMessengerEXT(mInstance, &debugMessengerCreateInfo, nullptr, &mDebugMessenger));
    }

    /* ======= Surface ======= */
    mSurface = VulkanPlatform::CreateSurface(mInstance);

    /* ======= Physical Device ======= */
    const char* deviceExtensions[] {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    u32 deviceCount;
    VK_CHECK(vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr));
    NTHROW_IF(deviceCount == 0, "Failed to find GPU with Vulkan support.");

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices(mInstance, &deviceCount, physicalDevices.data()));

    for (const auto device: physicalDevices)
    {
        VkPhysicalDeviceProperties deviceProps;
        vkGetPhysicalDeviceProperties(device, &deviceProps);

        if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            if (DeviceMeetsRequirements(device))
            {
                LOG_INFO("Selecting GPU: {} | Vulkan API version: {}.{}.{}",
                         deviceProps.deviceName,
                         VK_API_VERSION_MAJOR(deviceProps.apiVersion),
                         VK_API_VERSION_MINOR(deviceProps.apiVersion),
                         VK_API_VERSION_PATCH(deviceProps.apiVersion));

                vkGetPhysicalDeviceFeatures(device, &mDevice.DeviceFeatures);
                vkGetPhysicalDeviceMemoryProperties(device, &mDevice.DeviceMemoryProps);
                mDevice.DeviceProps = deviceProps;
                mDevice.PhysicalDevice = device;
                break;
            }
        }
    }
    NTHROW_IF(!mDevice.PhysicalDevice, "Could not find a GPU that meets the requirements.");

    /* ======= Logical Device ======= */
    std::unordered_set<u32> uniqueQueueFamilies { mDevice.QueueFamilyIndex.Graphics.value(), mDevice.QueueFamilyIndex.Present.value() };
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    queueCreateInfos.reserve(uniqueQueueFamilies.size());
    for (u32 uniqueQueueFamily: uniqueQueueFamilies)
    {
        f32 queuePriority { 1.0f };
        VkDeviceQueueCreateInfo queueCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = uniqueQueueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        };
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures {

    };

    VkDeviceCreateInfo deviceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = COUNT_OF(deviceExtensions),
        .ppEnabledExtensionNames = deviceExtensions,
        .pEnabledFeatures = &deviceFeatures
    };
    VK_CHECK(vkCreateDevice(mDevice.PhysicalDevice, &deviceCreateInfo, nullptr, &mDevice.LogicalDevice));
    vkGetDeviceQueue(mDevice.LogicalDevice, mDevice.QueueFamilyIndex.Present.value(), 0, &mDevice.PresentQueue);
    vkGetDeviceQueue(mDevice.LogicalDevice, mDevice.QueueFamilyIndex.Graphics.value(), 0, &mDevice.GraphicsQueue);
}

bool VulkanPlugin::DeviceMeetsRequirements(VkPhysicalDevice device)
{
    /* ======= Queue Families ======= */
    mDevice.QueueFamilyIndex.Reset();

    u32 queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (u32 i { 0 }; i < queueFamilyCount; ++i)
    {
        const auto& queueFamily { queueFamilies[i] };

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            mDevice.QueueFamilyIndex.Graphics = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {

        }

        if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
        {

        }

        VkBool32 supportsPresent { VK_FALSE };
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &supportsPresent));
        if (supportsPresent)
        {
            mDevice.QueueFamilyIndex.Present = i;
        }
    }
    if (!mDevice.QueueFamilyIndex.IsComplete())
        return false;

    /* ======= Device Extensions ======= */
    const char* deviceExtensions[] {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    u32 availableExtensionCount;
    VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, nullptr));

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, availableExtensions.data()));

    for (const char* extension: deviceExtensions)
    {
        bool available { false };
        for (const auto& availableExtension: availableExtensions)
        {
            if (!strcmp(availableExtension.extensionName, extension))
            {
                available = true;
            }
        }
        if (!available)
            return false;
    }

    /* ======= SwapChain Support ======= */
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &mSwapChainSupport.Capabilities));

    u32 formatCount;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr));
    if (formatCount < 1)
        return false;
    mSwapChainSupport.SurfaceFormats.resize(formatCount);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, mSwapChainSupport.SurfaceFormats.data()));

    u32 presentModeCount;
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr));
    if (presentModeCount < 1)
        return false;
    mSwapChainSupport.PresentModes.resize(presentModeCount);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, mSwapChainSupport.PresentModes.data()));

    return true;
}

VulkanPlugin::~VulkanPlugin()
{
    LOG_TRACE("Shutting down Vulkan renderer...");
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    vkDestroyDevice(mDevice.LogicalDevice, nullptr);

#ifndef NDEBUG
    auto vkDestroyDebugUtilsMessengerEXT { reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT")) };
    vkDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif

    vkDestroyInstance(mInstance, nullptr);
}

RendererPlugin* CreatePlugin()
{
    return new VulkanPlugin();
}

void DestroyPlugin(RendererPlugin* plugin)
{
    delete plugin;
}