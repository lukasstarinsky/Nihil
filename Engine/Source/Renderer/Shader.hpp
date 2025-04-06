#pragma once

enum class ShaderType
{
    Vertex = 0,
    Fragment
};

class Shader
{
public:
    Shader() = default;
    virtual ~Shader() = 0;

    static auto Create(std::string_view fileName);
};