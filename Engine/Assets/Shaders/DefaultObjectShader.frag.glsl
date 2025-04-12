#version 460 core

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
    outColor = texture(uTexture, inTexCoord) * vec4(1.0);
}