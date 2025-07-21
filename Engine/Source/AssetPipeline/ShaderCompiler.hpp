#pragma once

#include "Core/Defines.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"

namespace ShaderCompiler
{
    auto GlslToSpv(const std::string& glslSourceCode, ShaderStage shaderStage, RendererAPI api = Renderer::GetApi()) -> std::vector<u32>;
//    auto SPIRVToHLSL(const std::vector<u32> spirv) -> std::string;
}