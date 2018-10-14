#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;

void main()
{
	float hwDepth = texture(renderedTexture, UV).r;
	color = vec4(vec3(hwDepth), 1.0f);
}