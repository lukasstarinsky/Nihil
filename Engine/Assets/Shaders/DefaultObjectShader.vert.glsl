#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 outColor;

layout(binding = 0) uniform CameraUniformBuffer
{
    mat4 uProjection;
    mat4 uView;
};

#ifdef OPENGL
layout(location = 0) uniform mat4 uModel;
#endif

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
    outColor = inColor;
}