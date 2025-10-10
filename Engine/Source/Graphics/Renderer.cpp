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
    BufferCreateInfo cameraUBCreateInfo {
        .Type = BufferType::Uniform,
        .Data = nullptr,
        .Size = 2 * sizeof(Mat4f),
        .UniformBinding = CAMERA_UB_DEFAULT_BINDING
    };
    sState.CameraUniformBuffer = Buffer::Create(cameraUBCreateInfo);

    EventDispatcher::AddListener<ApplicationEvent>(OnAppEvent);
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

auto Renderer::OnAppEvent(const ApplicationEvent& e) -> bool
{
    ASSERT(sRendererBackend);
    if (e.Type == EventType::ApplicationResize)
    {
        sRendererBackend->OnResize(e.Width, e.Height);
    }
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

void Renderer::Draw(const MeshPtr& mesh, u32 subMeshIndex)
{
    ASSERT(sRendererBackend);
    sRendererBackend->Draw(mesh, subMeshIndex);
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

auto Shader::Create(const ShaderCreateInfo& shaderCreateInfo) -> ShaderPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateShader(shaderCreateInfo);
}

auto Material::Create(const MaterialCreateInfo& materialCreateInfo) -> MaterialPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateMaterial(materialCreateInfo);
}

auto Buffer::Create(const BufferCreateInfo& bufferCreateInfo) -> BufferPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateBuffer(bufferCreateInfo);
}

auto Mesh::Create(const MeshCreateInfo& meshCreateInfo) -> MeshPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateMesh(meshCreateInfo);
}

auto Texture::Create(const TextureCreateInfo& textureCreateInfo) -> TexturePtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateTexture(textureCreateInfo);
}