#pragma once

enum class UniformBinding : i32
{
    None = -1,
    Camera,
    Object,
    Material,
};

struct UniformBufferCreateInfo
{
    const void* Data {};
    i32 Size {};
    UniformBinding UniformBinding = UniformBinding::None;
};

class UniformBuffer;
using UniformBufferPtr = std::shared_ptr<UniformBuffer>;

class UniformBuffer
{
public:
    virtual ~UniformBuffer() = default;

    virtual void Bind() const = 0;
    virtual void SetData(const void* data, i32 size, u32 offset = 0) const = 0;

    static auto Create(const UniformBufferCreateInfo& createInfo) -> UniformBufferPtr;
};