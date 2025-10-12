#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;

layout(std140, binding = 0) uniform CameraUniformBuffer
{
    mat4 uProjection;
    mat4 uView;
};

layout(std140, binding = 1) uniform ObjectUniformBuffer
{
    mat4 uModel;
};

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
    outTexCoord = inTexCoord;
}