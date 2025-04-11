#include <shaderc/shaderc.hpp>

#include "ShaderCompiler.hpp"
#include "Renderer.hpp"

static auto ShaderTypeToShaderc(ShaderType shaderType) -> shaderc_shader_kind
{
    switch (shaderType)
    {
        case ShaderType::Vertex: return shaderc_glsl_vertex_shader;
        case ShaderType::Fragment: return shaderc_glsl_fragment_shader;
    }

    return shaderc_glsl_vertex_shader;
}

auto ShaderCompiler::GlslToSpv(std::string_view filePath, ShaderType shaderType) -> std::vector<u32>
{
    auto sourceCode = File::Read(filePath);
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    if (Renderer::GetApi() == RendererAPI::OpenGL)
    {
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        options.AddMacroDefinition("OPENGL");
    }

    shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(sourceCode, ShaderTypeToShaderc(shaderType), "Shader", options);
    Ensure(module.GetCompilationStatus() == shaderc_compilation_status_success, "Failed to compiler shader to Spv. {}", module.GetErrorMessage());

    return {module.begin(), module.end()};
}