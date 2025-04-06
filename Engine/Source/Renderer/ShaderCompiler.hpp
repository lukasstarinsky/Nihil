#pragma once

#include "Shader.hpp"

namespace ShaderCompiler
{
    auto GlslToSpv(const std::string& glslSource, ShaderType shaderType) -> std::vector<u32>;
//    auto SPIRVToHLSL(const std::vector<u32> spirv) -> std::string;
}