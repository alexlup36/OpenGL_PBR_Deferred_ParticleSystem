#version 330 core

in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec3 vertexNormal;
in layout(location = 2) vec2 vertexTexCoord;
in layout(location = 3) vec3 vertexTangent;
in layout(location = 4) vec3 vertexBitangent;

out VS_OUT
{
	vec3 normalW;
	vec3 vertexW;
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
}