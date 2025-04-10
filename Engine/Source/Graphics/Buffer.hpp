#pragma once

#define CAMERA_UB_DEFAULT_BINDING 0

enum class BufferType
{
    Vertex = 0,
    Index,
    Uniform
};

class Buffer;
using BufferPtr = std::shared_ptr<Buffer>;

class Buffer
{
public:
    virtual ~Buffer() = default;

    virtual void SetData(const void* data, i32 size, i32 offset) const = 0;

    static auto Create(BufferType bufferType, const void* data, i32 size, i32 uniformBinding = CAMERA_UB_DEFAULT_BINDING) -> BufferPtr;
};