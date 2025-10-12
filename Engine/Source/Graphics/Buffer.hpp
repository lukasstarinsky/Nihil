#pragma once

#define CAMERA_UB_DEFAULT_BINDING 0

enum class BufferType
{
    Vertex = 0,
    Index,
    Uniform
};

struct BufferCreateInfo
{
    BufferType Type;
    const void* Data;
    i32 Size;
    i32 UniformBinding = CAMERA_UB_DEFAULT_BINDING;
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