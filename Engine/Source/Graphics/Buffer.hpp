#pragma once

enum class BufferFlags
{
    None = 0,
    DynamicStorage
};

struct BufferCreateInfo
{
    BufferFlags Flags = BufferFlags::None;
    const void* Data {};
    i32 Size {};
};

class Buffer;
using BufferPtr = std::shared_ptr<Buffer>;

class Buffer
{
public:
    virtual ~Buffer() = default;

    virtual void Bind() const = 0;
    virtual auto GetSize() const -> i32 = 0;
    virtual void SetData(const void* data, i32 size, i32 offset) const = 0;

    static auto Create(const BufferCreateInfo& createInfo) -> BufferPtr;
};