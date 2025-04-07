#pragma once

#include "Core/Defines.hpp"
#include "Shader.hpp"

namespace ShaderCompiler
{
    auto NIHIL_API GlslToSpv(std::string_view filePath, ShaderType shaderType) -> std::vector<u32>;
//    auto SPIRVToHLSL(const std::vector<u32> spirv) -> std::string;
}