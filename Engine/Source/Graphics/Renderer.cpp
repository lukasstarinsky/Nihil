#include "Renderer.hpp"
#include "Platform/DynamicLibrary.hpp"

struct RendererState
{
    BufferPtr CameraUniformBuffer;
    BufferPtr ObjectUniformBuffer;
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

    BufferCreateInfo cameraUBOCreateInfo {
        .Type = BufferType::Uniform,
        .Data = nullptr,
        .Size = sizeof(CameraData),
        .UniformBinding = UniformBinding::Camera
    };
    sState.CameraUniformBuffer = Buffer::Create(cameraUBOCreateInfo);

    BufferCreateInfo objectUBOCreateInfo {
        .Type = BufferType::Uniform,
        .Data = nullptr,
        .Size = sizeof(ObjectData),
        .UniformBinding = UniformBinding::Object
    };
    sState.ObjectUniformBuffer = Buffer::Create(objectUBOCreateInfo);

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
    ASSERT(sRendererBackend);
    CameraData cameraData {
        .Projection = camera.GetProjectionMatrix(),
        .View = camera.GetViewMatrix()
    };
    sState.CameraUniformBuffer->SetData(&cameraData, sizeof(CameraData), 0);
}

void Renderer::Draw(const MeshPtr& mesh, const Mat4f& model, u32 subMeshIndex)
{
    ASSERT(sRendererBackend);

    ObjectData objectData {
        .Model = model
    };
    sState.ObjectUniformBuffer->SetData(&objectData, sizeof(ObjectData), 0);

    mesh->Bind();
    if (subMeshIndex != UINT32_MAX)
    {
        const auto& subMesh = mesh->GetSubMeshes()[subMeshIndex];
        auto material = mesh->GetMaterial(subMesh.MaterialIndex);
        material->Bind();
        sRendererBackend->Draw(subMesh);
    }
    else
    {
        for (const auto& subMesh: mesh->GetSubMeshes())
        {
            auto material = mesh->GetMaterial(subMesh.MaterialIndex);
            material->Bind();
            sRendererBackend->Draw(subMesh);
        }
    }
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

auto Shader::Create(const ShaderCreateInfo& createInfo) -> ShaderPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateShader(createInfo);
}

auto Material::Create(const MaterialCreateInfo& createInfo) -> MaterialPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateMaterial(createInfo);
}

auto Buffer::Create(const BufferCreateInfo& createInfo) -> BufferPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateBuffer(createInfo);
}

auto Mesh::Create(const MeshCreateInfo& createInfo) -> MeshPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateMesh(createInfo);
}

auto Texture::Create(const TextureCreateInfo& createInfo) -> TexturePtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateTexture(createInfo);
}