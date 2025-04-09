#include "Renderer.hpp"
#include "Platform/DynamicLibrary.hpp"

static DynamicLibrary sRendererModule;
static RendererBackend* sRendererBackend;
static BufferPtr sCameraUniformBuffer;

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

    sCameraUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Mat4f), 0);
}

void Renderer::Shutdown()
{
    ASSERT(sRendererBackend);
    auto DestroyPluginFn = sRendererModule.GetSymbol<Renderer::DestroyPluginFn>("DestroyPlugin");
    DestroyPluginFn(sRendererBackend);
}

void Renderer::BeginFrame(f32 r, f32 g, f32 b, f32 a)
{
    ASSERT(sRendererBackend);
    sRendererBackend->BeginFrame(r, g, b, a);

    // Temp
    static f32 pos = 0.01f;
    pos += 0.01f;
    auto translation = Mat4f::Translation({pos, pos, 0.0f});
    sCameraUniformBuffer->SetData(translation.Data(), sizeof(Mat4f));
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

auto Mesh::Create() -> MeshPtr
{
    ASSERT(sRendererBackend);
    return sRendererBackend->CreateMesh();
}