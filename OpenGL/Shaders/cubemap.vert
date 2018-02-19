#version 330 core

layout (location = 0) in vec3 vPos;

out vec3 textureDir;

void main()
{
    gl_Position = vec4(vPos, 1.0f);

	textureDir = vPos;
}