#include <shaderc/shaderc.hpp>

#include "ShaderCompiler.hpp"
#include "Renderer.hpp"

static auto ShaderStageToShaderc(ShaderStage shaderType) -> shaderc_shader_kind
{
    switch (shaderType)
    {
        case ShaderStage::Vertex: return shaderc_glsl_vertex_shader;
        case ShaderStage::Fragment: return shaderc_glsl_fragment_shader;
    }

    return shaderc_glsl_vertex_shader;
}

auto ShaderCompiler::GlslToSpv(const std::string& glslSourceCode, ShaderStage shaderStage) -> std::vector<u32>
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    if (Renderer::GetApi() == RendererAPI::OpenGL)
    {
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        options.AddMacroDefinition("OPENGL");
    }

    shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(glslSourceCode, ShaderStageToShaderc(shaderStage), "Shader", options);
    Ensure(module.GetCompilationStatus() == shaderc_compilation_status_success, "Failed to compiler shader to Spv. {}", module.GetErrorMessage());

    return {module.begin(), module.end()};
}