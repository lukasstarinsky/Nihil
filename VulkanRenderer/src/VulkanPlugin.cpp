#include <unordered_set>
#include "VulkanPlugin.hpp"
#include "Platform/VulkanPlatform.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
    if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        return VK_FALSE;

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

VulkanPlugin::VulkanPlugin(i32 width, i32 height)
{
    LOG_TRACE("Initializing Vulkan renderer...");

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
#ifndef NDEBUG
    const char* instanceLayers[] {
        "VK_LAYER_KHRONOS_validation"
    };

    u32 layerCount;
    VK_CHECK(vkEnumerateInstanceLayerProperties, &layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties, &layerCount, availableLayers.data());

    bool validationSupported { false };
    for (const auto& layerProperties: availableLayers)
    {
        if (!strcmp(instanceLayers[0], layerProperties.layerName))
        {
            validationSupported = true;
            break;
        }
    }
    if (!validationSupported)
    {
        NTHROW("Vulkan validation layers are not available");
    }

    instanceCreateInfo.enabledLayerCount = COUNT_OF(instanceLayers);
    instanceCreateInfo.ppEnabledLayerNames = instanceLayers;
#endif

    VK_CHECK(vkCreateInstance, &instanceCreateInfo, nullptr, &mContext.Instance);

    /* ======= Debug Messenger ======= */
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

    auto vkCreateDebugUtilsMessengerEXT { reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mContext.Instance, "vkCreateDebugUtilsMessengerEXT")) };
    VK_CHECK(vkCreateDebugUtilsMessengerEXT, mContext.Instance, &debugMessengerCreateInfo, nullptr, &mContext.DebugMessenger);
#endif

    /* ======= Surface ======= */
    mContext.Surface = VulkanPlatform::CreateSurface(mContext.Instance);

    /* ======= Physical Device ======= */
    const char* deviceExtensions[] {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    u32 deviceCount;
    VK_CHECK(vkEnumeratePhysicalDevices, mContext.Instance, &deviceCount, nullptr);

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices, mContext.Instance, &deviceCount, physicalDevices.data());

    for (const auto device: physicalDevices)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            if (DeviceMeetsRequirements(device))
            {
                LOG_INFO("Selecting GPU: {}", deviceProperties.deviceName);

                vkGetPhysicalDeviceFeatures(device, &mDevice.DeviceFeatures);
                mDevice.DeviceProperties = deviceProperties;
                mDevice.PhysicalDevice = device;
                break;
            }
        }
    }
    if (!mDevice.PhysicalDevice)
    {
        NTHROW("Could not find a GPU that meets the requirements.");
    }

    /* ======= Logical Device ======= */
    std::unordered_set<u32> uniqueQueueFamilies { mDevice.GraphicsQueueFamilyIndex, mDevice.PresentQueueFamilyIndex };
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
    VK_CHECK(vkCreateDevice, mDevice.PhysicalDevice, &deviceCreateInfo, nullptr, &mDevice.LogicalDevice);
    vkGetDeviceQueue(mDevice.LogicalDevice, mDevice.PresentQueueFamilyIndex, 0, &mDevice.PresentQueue);
    vkGetDeviceQueue(mDevice.LogicalDevice, mDevice.GraphicsQueueFamilyIndex, 0, &mDevice.GraphicsQueue);

    /* ======= SwapChain ======= */
    VkSurfaceFormatKHR selectedSurfaceFormat { mDevice.SurfaceFormats[0] };
    for (const auto& surfaceFormat: mDevice.SurfaceFormats)
    {
        if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            selectedSurfaceFormat = surfaceFormat;
            break;
        }
    }

    VkPresentModeKHR selectedPresentMode { VK_PRESENT_MODE_FIFO_KHR };
    for (auto presentMode: mDevice.PresentModes)
    {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            selectedPresentMode = presentMode;
            break;
        }
    }

    u32 imageCount { mDevice.Capabilities.minImageCount + 1 };
    if (mDevice.Capabilities.maxImageCount > 0 && mDevice.Capabilities.maxImageCount < imageCount)
    {
        imageCount = mDevice.Capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = mContext.Surface,
        .minImageCount = imageCount,
        .imageFormat = selectedSurfaceFormat.format,
        .imageColorSpace = selectedSurfaceFormat.colorSpace,
        // TODO: take from constructor args
        .imageExtent = mDevice.Capabilities.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = mDevice.Capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = selectedPresentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = nullptr
    };

    u32 queueFamilyIndices[] { mDevice.GraphicsQueueFamilyIndex, mDevice.PresentQueueFamilyIndex };
    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT,
        swapChainCreateInfo.queueFamilyIndexCount = COUNT_OF(queueFamilyIndices);
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    VK_CHECK(vkCreateSwapchainKHR, mDevice.LogicalDevice, &swapChainCreateInfo, nullptr, &mSwapChain);
}

bool VulkanPlugin::DeviceMeetsRequirements(VkPhysicalDevice device)
{
    /* ======= Queue Families ======= */
    mDevice.GraphicsQueueFamilyIndex = -1;
    mDevice.PresentQueueFamilyIndex = -1;

    u32 queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (u32 i { 0 }; i < queueFamilyCount; ++i)
    {
        const auto& queueFamily { queueFamilies[i] };

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            mDevice.GraphicsQueueFamilyIndex = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {

        }

        if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
        {

        }

        VkBool32 supportsPresent { VK_FALSE };
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR, device, i, mContext.Surface, &supportsPresent);
        if (supportsPresent)
        {
            mDevice.PresentQueueFamilyIndex = i;
        }
    }
    if (mDevice.GraphicsQueueFamilyIndex == -1u || mDevice.PresentQueueFamilyIndex == -1u)
        return false;

    /* ======= Device Extensions ======= */
    const char* deviceExtensions[] {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    u32 availableExtensionCount;
    VK_CHECK(vkEnumerateDeviceExtensionProperties, device, nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    VK_CHECK(vkEnumerateDeviceExtensionProperties, device, nullptr, &availableExtensionCount, availableExtensions.data());

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
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR, device, mContext.Surface, &mDevice.Capabilities);

    u32 formatCount;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR, device, mContext.Surface, &formatCount, nullptr);
    if (formatCount < 1)
        return false;
    mDevice.SurfaceFormats.resize(formatCount);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR, device, mContext.Surface, &formatCount, mDevice.SurfaceFormats.data());

    u32 presentModeCount;
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR, device, mContext.Surface, &presentModeCount, nullptr);
    if (presentModeCount < 1)
        return false;
    mDevice.PresentModes.resize(presentModeCount);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR, device, mContext.Surface, &presentModeCount, mDevice.PresentModes.data());

    return true;
}

VulkanPlugin::~VulkanPlugin()
{
    LOG_TRACE("Shutting down Vulkan renderer...");
    vkDestroySwapchainKHR(mDevice.LogicalDevice, mSwapChain, nullptr);
    vkDestroySurfaceKHR(mContext.Instance, mContext.Surface, nullptr);
    vkDestroyDevice(mDevice.LogicalDevice, nullptr);

#ifndef NDEBUG
    auto vkDestroyDebugUtilsMessengerEXT { reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mContext.Instance, "vkDestroyDebugUtilsMessengerEXT")) };
    vkDestroyDebugUtilsMessengerEXT(mContext.Instance, mContext.DebugMessenger, nullptr);
#endif

    vkDestroyInstance(mContext.Instance, nullptr);
}

RendererPlugin* CreatePlugin(i32 width, i32 height)
{
    return new VulkanPlugin(width, height);
}

void DestroyPlugin(RendererPlugin* plugin)
{
    delete plugin;
}