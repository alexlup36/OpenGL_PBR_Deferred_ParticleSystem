#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out VS_OUT
{
	vec3 normalW;
	vec3 vertexW;
	vec2 vertexUV;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat;

void main()
{
	// Calculate fragment position in screen space
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
	
	// Calculate vertex position in world coordinates
	vs_out.vertexW 	= vec3(model * vec4(vertexPosition, 1.0f));
	// Calculate normal direction in world coordinates
	vs_out.normalW 	= normalize(mat3(normalMat) * vertexNormal);
	// Vertex uv
	vs_out.vertexUV = vertexTexCoord;
}