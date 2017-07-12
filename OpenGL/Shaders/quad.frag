#version 330 core

in vec2 UV;

out vec4 fragmentColor;

uniform sampler2D renderedTexture;

void main()
{
    fragmentColor = vec4(texture(renderedTexture, UV).xyz, 1.0f);
}