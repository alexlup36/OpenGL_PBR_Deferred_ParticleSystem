#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;

out VS_OUT
{
    vec3 wsPosition;
    vec2 uv;
    mat3 tbn;
} vs_out;

uniform mat4 model;
uniform mat4 normalMat;

void calculateTNBMatrix()
{
	// Transform the normal, tangent and calculate binormals
	vec3 n = normalize(mat3(normalMat) * vertexNormal);
	vec3 t = normalize(mat3(normalMat) * vertexTangent);
	
	// Make sure the t and n vectors are orthogonal
	t = normalize(t - dot(t, n) * n);
	
	vec3 b = normalize(cross(t, n)); // Order important
	
	// Create the TBN matrix
	vs_out.tbn = mat3(t, b, n);
}

void main()
{
    vs_out.wsPosition = (model * vec4(vertexPosition, 1.0f)).xyz;
    vs_out.uv = vertexTexCoord;
    calculateTNBMatrix();
}