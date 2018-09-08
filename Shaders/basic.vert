#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec3 vCol;

uniform mat4 MVP;

out vec3 vertexColor;

void main()
{
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    vertexColor = vCol;
}