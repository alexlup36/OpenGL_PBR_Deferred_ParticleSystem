#version 330 core

layout (location = 0) in vec3 vPos;

out vec2 UV;

void main()
{
    gl_Position = vec4(vPos, 1.0f);

	// Convert vertex position [-1.0f - 1.0f] to texture coordinates [0.0f, 1.0f]
    UV = (vPos.xy + vec2(1.0f, 1.0f)) * 0.5f;
}