#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 vPosWorld;
out vec3 vNorm;

uniform mat4 model;
uniform mat4 normalMat;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vPosWorld = vec3(model * vec4(vPos, 1.0f));
	vNorm = vec3(normalMat * vec4(vNormal, 0.0f));

    gl_Position = projection * view * vec4(vPosWorld, 1.0);
}