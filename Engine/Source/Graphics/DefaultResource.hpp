#pragma once

#include "Platform/UUID.hpp"

namespace DefaultResource
{
    constexpr std::string_view ObjectVertexShaderFile = "DefaultObject.vs";
    constexpr std::string_view ObjectFragmentShaderFile = "DefaultObject.fs";
    constexpr std::string_view ObjectMaterialName = "DefaultObjectMaterial";
    constexpr std::string_view TextureFile = "default.png";

    constexpr Nihil::UUID ObjectVertexShader(0);
    constexpr Nihil::UUID ObjectFragmentShader(1);
    constexpr Nihil::UUID ObjectMaterial(10);
    constexpr Nihil::UUID Texture(20);
}