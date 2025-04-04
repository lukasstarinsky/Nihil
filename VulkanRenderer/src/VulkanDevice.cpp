#include "VulkanDevice.hpp"

VulkanDevice::VulkanDevice(const VulkanContext& context)
{
    const char* deviceExtensions[] {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    u32 deviceCount;
    VK_CHECK(vkEnumeratePhysicalDevices(context.GetInstance(), &deviceCount, nullptr));

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices(context.GetInstance(), &deviceCount, physicalDevices.data()));

    for (const auto device: physicalDevices)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            if (IsDeviceSuitable(device, context.GetSurface()))
            {
                Logger::Info("Selecting GPU: {}", deviceProperties.deviceName);
                mPhysicalDevice = device;
                break;
            }
        }
    }
    Ensure(mPhysicalDevice, "Could not find a GPU that meets the requirements.");

    /* ======= Logical Device ======= */
    std::unordered_set<u32> uniqueQueueFamilies { mGraphicsFamilyIndex, mPresentFamilyIndex };
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    queueCreateInfos.reserve(uniqueQueueFamilies.size());
    for (u32 uniqueQueueFamily: uniqueQueueFamilies)
    {
        f32 queuePriority = 1.0f;
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
    VK_CHECK(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice));
    vkGetDeviceQueue(mLogicalDevice, mPresentFamilyIndex, 0, &mPresentQueue);
    vkGetDeviceQueue(mLogicalDevice, mGraphicsFamilyIndex, 0, &mGraphicsQueue);
}

VulkanDevice::~VulkanDevice()
{
    vkDestroyDevice(mLogicalDevice, nullptr);
}

auto VulkanDevice::IsDeviceSuitable(const VkPhysicalDevice& device, VkSurfaceKHR surface) -> bool
{
    /* ======= Queue Families ======= */
    mGraphicsFamilyIndex = -1;
    mPresentFamilyIndex = -1;

    u32 queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (u32 i = 0; i < queueFamilyCount; ++i)
    {
        const auto& queueFamily = queueFamilies[i];

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            mGraphicsFamilyIndex = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {

        }

        if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
        {

        }

        VkBool32 supportsPresent = VK_FALSE;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supportsPresent));
        if (supportsPresent)
        {
            mPresentFamilyIndex = i;
        }
    }
    if (mGraphicsFamilyIndex == -1u || mPresentFamilyIndex == -1u)
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
        bool available = false;
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
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &mCapabilities));

    u32 formatCount;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr));
    if (formatCount < 1)
        return false;
    mSurfaceFormats.resize(formatCount);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, mSurfaceFormats.data()));

    u32 presentModeCount;
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr));
    if (presentModeCount < 1)
        return false;
    mPresentModes.resize(presentModeCount);
    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, mPresentModes.data()));

    return true;
}

auto VulkanDevice::GetLogicalDevice() const -> VkDevice
{
    return mLogicalDevice;
}

auto VulkanDevice::GetSurfaceFormats() const -> const std::vector<VkSurfaceFormatKHR>&
{
    return mSurfaceFormats;
}

auto VulkanDevice::GetPresentModes() const -> const std::vector<VkPresentModeKHR>&
{
    return mPresentModes;
}

auto VulkanDevice::GetCapabilities() const -> const VkSurfaceCapabilitiesKHR&
{
    return mCapabilities;
}

auto VulkanDevice::GetQueueFamilyIndices() const -> std::array<u32, 2>
{
    return {mGraphicsFamilyIndex, mPresentFamilyIndex };
}