#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat;
uniform vec3 viewPos;

void main()
{
	gl_Position = vec4(vertexPosition, 1.0f);
	uv = vertexTexCoord;
}