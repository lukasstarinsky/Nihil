#version 460 core

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

layout(std140, binding = 2) uniform MaterialUniformBuffer
{
    vec4 uBaseColor;
};

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    outColor = uBaseColor;
}