#include "VulkanBackend.hpp"

VulkanBackend::VulkanBackend(const ApplicationConfig& appConfig, const PlatformState& platformState)
    : mPlatformState{platformState}
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

    const char* instanceExtensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef NIHIL_PLATFORM_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif
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

    Ensure(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance) == VK_SUCCESS, "Failed to create Vulkan instance");
}

VulkanBackend::~VulkanBackend()
{
}

auto VulkanBackend::GetApi() const -> RendererAPI
{
    return RendererAPI::Vulkan;
}

auto VulkanBackend::GetApiString() const -> const char*
{
    return "Vulkan";
}

void VulkanBackend::BeginFrame(f32 r, f32 g, f32 b, f32 a) const
{
}

void VulkanBackend::OnResize(i32 width, i32 height) const
{
}

void VulkanBackend::EndFrame() const
{
}

auto VulkanBackend::CreateShader(const ShaderSpecification& shaderSpec) const -> ShaderPtr
{
    return nullptr;
}

auto VulkanBackend::CreateMaterial(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) const -> MaterialPtr
{
    return nullptr;
}

auto VulkanBackend::CreateBuffer(BufferType bufferType, const void* data, i32 size, i32 uniformBinding) const -> BufferPtr
{
    return nullptr;
}

auto VulkanBackend::CreateMesh(const MeshSpecification& meshSpec) const -> MeshPtr
{
    return nullptr;
}

auto VulkanBackend::CreateTexture(const TextureSpecification& textureSpec) const -> TexturePtr
{
    return nullptr;
}

void VulkanBackend::Draw(const MeshPtr& mesh) const
{
}

extern "C"
{
    NIHIL_API auto CreatePlugin(const ApplicationConfig& appConfig, const PlatformState& platformState, std::exception_ptr& exceptionPtr) -> RendererBackend*
    {
        try
        {
            return new VulkanBackend(appConfig, platformState);
        }
        catch (...)
        {
            exceptionPtr = std::current_exception();
            return nullptr;
        }
    }

    NIHIL_API void DestroyPlugin(VulkanBackend* plugin)
    {
        delete plugin;
    }
}
