#version 460 core

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inVertexUV;
layout(location = 2) in vec2 inInstancePosition;
layout(location = 3) in vec2 inInstanceSize;

layout(location = 0) out vec4 outFragColor;

layout(std140, binding = 0) uniform CameraUniformBuffer
{
    mat4 uProjection;
    mat4 uView;
};

void main()
{
    vec2 scaledPos = inVertexPosition.xy * inInstanceSize + inInstancePosition;
    gl_Position = uProjection * vec4(scaledPos, 0.0, 1.0);
    outFragColor = vec4(1.0, 0.0, 1.0, 1.0);
}