#include "VulkanContext.hpp"

#include <list>

VulkanContext::VulkanContext(const ApplicationConfig& appConfig, const PlatformState& platformState)
{
    CreateInstance(appConfig);
    CreateSurface(platformState);
    CreatePhysicalDevice();
    CreateLogicalDevice();
}

VulkanContext::~VulkanContext()
{
    vkDestroySurfaceKHR(this->Instance, this->Surface, nullptr);
    vkDestroyDevice(this->Device, nullptr);
    vkDestroyInstance(this->Instance, nullptr);
}

void VulkanContext::CreateInstance(const ApplicationConfig& appConfig)
{
    VkApplicationInfo appInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = appConfig.Name.c_str(),
        .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
        .pEngineName = "Nihil Engine",
        .engineVersion = VK_MAKE_VERSION(0, 1, 0),
        .apiVersion = VK_API_VERSION_1_3
    };

    std::vector<const char*> instanceExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef NIHIL_PLATFORM_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif
    };

    std::vector<const char*> instanceLayers;

#ifndef NDEBUG
    u32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto& layer: availableLayers)
    {
        if (strcmp(layer.layerName, "VK_LAYER_KHRONOS_validation") == 0)
        {
//            instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            instanceLayers.push_back(layer.layerName);
            break;
        }
    }
    Ensure(!instanceLayers.empty(), "Validation layers are not supported.");
#endif

    VkInstanceCreateInfo instanceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<u32>(instanceLayers.size()),
        .ppEnabledLayerNames = instanceLayers.data(),
        .enabledExtensionCount = static_cast<u32>(instanceExtensions.size()),
        .ppEnabledExtensionNames = instanceExtensions.data()
    };

    Ensure(vkCreateInstance(&instanceCreateInfo, nullptr, &this->Instance) == VK_SUCCESS, "Failed to create Vulkan instance");
}

void VulkanContext::CreateSurface(const PlatformState& platformState)
{
#ifdef NIHIL_PLATFORM_WINDOWS
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .hinstance = platformState.Instance,
        .hwnd = platformState.WindowHandle
    };
    Ensure(vkCreateWin32SurfaceKHR(this->Instance, &surfaceCreateInfo, nullptr, &this->Surface) == VK_SUCCESS, "Failed to create Vulkan surface");
#endif
}

void VulkanContext::CreatePhysicalDevice()
{
    u32 deviceCount;
    vkEnumeratePhysicalDevices(this->Instance, &deviceCount, nullptr);
    Ensure(deviceCount > 0, "No Vulkan physical devices found.");

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(this->Instance, &deviceCount, physicalDevices.data());

    for (const auto& device : physicalDevices)
    {
        this->GraphicsQueueFamilyIndex = -1u;
        this->PresentQueueFamilyIndex = -1u;

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            u32 queueFamilyCount;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            for (u32 i = 0; i < queueFamilyCount; ++i)
            {
                if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    this->GraphicsQueueFamilyIndex = i;
                }

                VkBool32 supportsPresent = false;
                Ensure(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->Surface, &supportsPresent) == VK_SUCCESS, "Failed to check surface support for physical device");

                if (supportsPresent)
                {
                    this->PresentQueueFamilyIndex = i;
                }
            }

            if (this->GraphicsQueueFamilyIndex != -1u && this->PresentQueueFamilyIndex != -1u)
            {
                this->PhysicalDevice = device;
                break;
            }
        }
    }
    Ensure(this->PhysicalDevice != VK_NULL_HANDLE, "No suitable Vulkan physical device found.");
}

void VulkanContext::CreateLogicalDevice()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::unordered_set<u32> uniqueQueueFamilies { this->GraphicsQueueFamilyIndex, this->PresentQueueFamilyIndex };

    for (const auto queueFamily: uniqueQueueFamilies)
    {
        f32 queuePriority = 1.0f;
        queueCreateInfos.push_back({
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        });
    }

    VkPhysicalDeviceFeatures deviceFeatures = {

    };

    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo deviceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<u32>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data(),
        .pEnabledFeatures = &deviceFeatures
    };

    Ensure(vkCreateDevice(this->PhysicalDevice, &deviceCreateInfo, nullptr, &this->Device) == VK_SUCCESS, "Failed to create Vulkan logical device");

    vkGetDeviceQueue(this->Device, this->GraphicsQueueFamilyIndex, 0, &this->GraphicsQueue);
    if (this->PresentQueueFamilyIndex != this->GraphicsQueueFamilyIndex)
    {
        vkGetDeviceQueue(this->Device, this->PresentQueueFamilyIndex, 0, &this->PresentQueue);
    }
    else
    {
        this->PresentQueue = this->GraphicsQueue;
    }
}