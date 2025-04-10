#include "Renderer.hpp"
#include "Platform/DynamicLibrary.hpp"

struct RendererState
{
    BufferPtr CameraUniformBuffer;
    ShaderPtr DefaultVertexShader;
    ShaderPtr DefaultFragmentShader;
    MaterialPtr DefaultMaterial;
};

static DynamicLibrary sRendererModule;
static RendererBackend* sRendererBackend;
static RendererState sState;

void Renderer::Initialize(const ApplicationConfig& config)
{
    if (!DynamicLibrary::Load(Renderer::ApiToModuleString(config.RendererAPI), &sRendererModule))
    {
        Throw("Failed to load plugin '{}'", Renderer::ApiToModuleString(config.RendererAPI));
    }

    if (!sRendererModule.LoadSymbol("CreatePlugin") || !sRendererModule.LoadSymbol("DestroyPlugin"))
    {
        Throw("Failed to load symbols of plugin '{}'.", Renderer::ApiToModuleString(config.RendererAPI));
    }

    std::exception_ptr exception;
    auto CreatePluginFn = sRendererModule.GetSymbol<Renderer::CreatePluginFn>("CreatePlugin");
    sRendererBackend = CreatePluginFn(config, exception);

    if (exception)
    {
        std::rethrow_exception(exception);
    }

    sState.CameraUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, 3 * sizeof(Mat4f), CAMERA_UB_DEFAULT_BINDING);
    sState.DefaultVertexShader = Shader::Create("Assets/Shaders/DefaultObjectShader.vert", ShaderType::Vertex);
    sState.DefaultFragmentShader = Shader::Create("Assets/Shaders/DefaultObjectShader.frag", ShaderType::Fragment);
    sState.DefaultMaterial = Material::Create(sState.DefaultVertexShader, sState.DefaultFragmentShader);

    ADD_EVENT_LISTENER(Event::ApplicationResize, OnResizeEvent);
}

void Renderer::Shutdown()
{
    ASSERT(sRendererBackend);
    auto DestroyPluginFn = sRendererModule.GetSymbol<Renderer::DestroyPluginFn>("DestroyPlugin");
    DestroyPluginFn(sRendererBackend);
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

    // Temp
    auto projection = Mat4f::Perspective(std::numbers::pi / 3.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    auto view = Mat4f::LookAt({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    static f32 rot = 0.0f;
    rot += 0.01f;
    auto model = Mat4f::RotateX(rot);
    sState.CameraUniformBuffer->SetData(projection.Data(), sizeof(Mat4f), 0);
    sState.CameraUniformBuffer->SetData(view.Data(), sizeof(Mat4f), sizeof(Mat4f));
    sState.CameraUniformBuffer->SetData(model.Data(), sizeof(Mat4f), sizeof(Mat4f) * 2);
}

void Renderer::EndFrame()
{
    ASSERT(sRendererBackend);
    sRendererBackend->EndFrame();
}

void Renderer::Draw(const MeshPtr& mesh)
{
    ASSERT(sRendererBackend);
    sRendererBackend->Draw(mesh);
}

auto Renderer::DefaultVertexShader() -> const ShaderPtr&
{
    return sState.DefaultVertexShader;
}

auto Renderer::DefaultFragmentShader() -> const ShaderPtr&
{
    return sState.DefaultFragmentShader;
}

auto Renderer::DefaultMaterial() -> const MaterialPtr&
{
    return sState.DefaultMaterial;
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

auto Shader::Create(const std::string& filePath, ShaderType shaderType) -> ShaderPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateShader(filePath, shaderType);
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

auto Mesh::Create(std::span<const Vertex> vertices, std::span<const Index> indices) -> MeshPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateMesh(vertices, indices);
}