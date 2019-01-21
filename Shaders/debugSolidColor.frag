#version 330 core

layout(location = 0) out vec4 fragmentColor;

in vec3 vPosWorld;
in vec3 vNorm;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec4 objectColor;

void main()
{
	// Ambient component
	float ambientStrength = 0.9f;
	vec3 ambientComponent = ambientStrength * lightColor;
	
	// Diffuse component
	vec3 normal = normalize(vNorm);
	vec3 lightDir = normalize(lightDir);
	float diffuse = max(dot(normal, lightDir), 0.0f);
	vec3 diffuseComponent = diffuse * lightColor;
	
	// Final colour
	vec3 lightComponents = (ambientComponent + diffuseComponent);
	fragmentColor = vec4(lightComponents, 1.0f) * objectColor;
}