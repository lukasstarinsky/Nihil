#pragma once

enum class BufferType
{
    Vertex = 0,
    Index,
    Uniform
};

enum class UniformBinding
{
    None = -1,
    Camera,
    Object,
    Material,
};

struct BufferCreateInfo
{
    BufferType Type;
    const void* Data;
    i32 Size;
    UniformBinding UniformBinding = UniformBinding::None;
};

class Buffer;
using BufferPtr = std::shared_ptr<Buffer>;

class Buffer
{
public:
    virtual ~Buffer() = default;

    virtual void SetData(const void* data, i32 size, i32 offset) const = 0;

    static auto Create(const BufferCreateInfo& createInfo) -> BufferPtr;
};