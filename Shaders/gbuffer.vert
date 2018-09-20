#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;

out vec3 wsPosition;
out vec2 uv;
out vec3 wsTangent;

uniform mat4 model;
uniform mat4 normalMat;

void main()
{
    wsPosition = (model * vec4(vertexPosition, 1.0f)).xyz;
    uv = vertexTexCoord;
    wsTangent = normalize(mat3(normalMat) * vertexTangent);
}