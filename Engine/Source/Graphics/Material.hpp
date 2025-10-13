#pragma once

#include "DefaultResource.hpp"
#include "Math/Math.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Resource.hpp"

struct MaterialParameter
{
    enum class Type : u32
    {
        Float = sizeof(f32),
        Vec3 = sizeof(Vec3f),
        Vec4 = sizeof(Vec4f),
        Mat4 = sizeof(Mat4f)
    };

    std::string Name;
    MaterialParameter::Type Type {};
    u32 Offset {};
};

struct MaterialSpecification
{
    Nihil::UUID UUID = DefaultResource::ObjectMaterial;
    Nihil::UUID VertexShaderUUID = DefaultResource::ObjectVertexShader;
    Nihil::UUID FragmentShaderUUID = DefaultResource::ObjectFragmentShader;
    std::vector<MaterialParameter> Layout;
};

struct MaterialCreateInfo
{
    ShaderPtr VertexShader;
    ShaderPtr FragmentShader;
    std::vector<MaterialParameter> Layout;
};

class Material;
using MaterialPtr = std::shared_ptr<Material>;

class Material : public Resource
{
public:
    using Specification = MaterialSpecification;

    virtual ~Material() = default;

    virtual void Bind() const = 0;

    static auto Create(const MaterialCreateInfo& createInfo) -> MaterialPtr;
protected:
    explicit Material(const MaterialCreateInfo& createInfo)
        : mLayout{createInfo.Layout}
    {
        for (const auto& param: mLayout)
        {
            mLayoutSize += static_cast<i32>(param.Type);
        }
    }

    std::vector<MaterialParameter> mLayout {};
    i32 mLayoutSize {};

    friend class MaterialInstance;
};