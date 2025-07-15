#include "Renderer.hpp"
#include "Platform/DynamicLibrary.hpp"

struct RendererState
{
    BufferPtr CameraUniformBuffer;
};

static DynamicLibrary sRendererModule;
static RendererBackend* sRendererBackend;
static RendererState sState;

void Renderer::Initialize(const ApplicationConfig& config)
{
    auto moduleName = Renderer::ApiToModuleString(config.RendererAPI);

    Logger::Trace("Initializing {}...", moduleName);
    if (!DynamicLibrary::Load(moduleName, &sRendererModule))
    {
        Throw("Failed to load plugin '{}'", moduleName);
    }

    if (!sRendererModule.LoadSymbol("CreatePlugin") || !sRendererModule.LoadSymbol("DestroyPlugin"))
    {
        Throw("Failed to load symbols of plugin '{}'.", moduleName);
    }

    std::exception_ptr exception;
    auto CreatePluginFn = sRendererModule.GetSymbol<Renderer::CreatePluginFn>("CreatePlugin");
    sRendererBackend = CreatePluginFn(config, Platform::GetState(), exception);

    if (exception)
    {
        std::rethrow_exception(exception);
    }

    // TODO: Move to scene
    sState.CameraUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, 2 * sizeof(Mat4f), CAMERA_UB_DEFAULT_BINDING);

    ADD_EVENT_LISTENER(Event::ApplicationResize, OnResizeEvent);
}

void Renderer::Shutdown()
{
    ASSERT(sRendererBackend);

    Logger::Trace("Shutting down renderer...");
    auto DestroyPluginFn = sRendererModule.GetSymbol<Renderer::DestroyPluginFn>("DestroyPlugin");
    DestroyPluginFn(sRendererBackend);
}

auto Renderer::GetApi() -> RendererAPI
{
    ASSERT(sRendererBackend);
    return sRendererBackend->GetApi();
}

auto Renderer::GetApiString() -> const char*
{
    ASSERT(sRendererBackend);
    return sRendererBackend->GetApiString();
}

auto Renderer::OnResizeEvent(const Event& e) -> bool
{
    ASSERT(sRendererBackend);
    const auto& appEvent = e.ApplicationEvent;
    sRendererBackend->OnResize(appEvent.Width, appEvent.Height);
    return false;
}

void Renderer::BeginFrame(f32 r, f32 g, f32 b, f32 a)
{
    ASSERT(sRendererBackend);
    sRendererBackend->BeginFrame(r, g, b, a);
}

void Renderer::EndFrame()
{
    ASSERT(sRendererBackend);
    sRendererBackend->EndFrame();
}

void Renderer::BeginScene(const Camera& camera)
{
    sState.CameraUniformBuffer->SetData(camera.GetProjectionMatrix().Data(), sizeof(Mat4f), 0);
    sState.CameraUniformBuffer->SetData(camera.GetViewMatrix().Data(), sizeof(Mat4f), sizeof(Mat4f));
}

void Renderer::Draw(const MeshPtr& mesh)
{
    ASSERT(sRendererBackend);
    sRendererBackend->Draw(mesh);
}

auto Renderer::ApiToModuleString(RendererAPI api) -> const char*
{
    switch (api)
    {
        case RendererAPI::Vulkan:       return "VulkanRenderer";
        case RendererAPI::OpenGL:       return "OpenGLRenderer";
        case RendererAPI::Direct3D11:   return "Direct3D11Renderer";
        case RendererAPI::Direct3D12:   return "Direct3D12Renderer";
        case RendererAPI::Metal:        return "MetalRenderer";
    }
}

auto Shader::Create(const ShaderSpecification& shaderSpec) -> ShaderPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateShader(shaderSpec);
}

auto Material::Create(const ShaderPtr& vertexShader, const ShaderPtr& fragmentShader) -> MaterialPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateMaterial(vertexShader, fragmentShader);
}

auto Buffer::Create(BufferType bufferType, const void* data, i32 size, i32 uniformBinding) -> BufferPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateBuffer(bufferType, data, size, uniformBinding);
}

auto Mesh::Create(const MeshSpecification& meshSpec) -> MeshPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateMesh(meshSpec);
}

auto Texture::Create(const TextureSpecification& textureSpec) -> TexturePtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateTexture(textureSpec);
}