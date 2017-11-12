#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexture;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

out vec3 vPosWorld;
out vec3 vNorm;
out vec2 vUV;
out vec3 vTan;
out vec3 vBitan;

uniform mat4 model;
uniform mat4 normalMat;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vPosWorld = vec3(model * vec4(vPos, 1.0f));
	vNorm = vec3(normalMat * vec4(vNormal, 0.0f));
	vUV = vTexture;
	vTan = vTangent;
	vBitan = vBitangent;

    gl_Position = projection * view * vec4(vPosWorld, 1.0);
}