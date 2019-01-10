#version 330 core

layout(location = 0) out vec4 fragmentColor;

in vec3 vPosWorld;
in vec3 vNorm;
in vec2 vUV;
in vec3 vTan;
in vec3 vBitan;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec4 objectColor;
uniform vec3 viewPos;
uniform float shininess;
uniform float specularStrength;

uniform sampler2D diffuseTexture1;
uniform sampler2D normalTexture1;
uniform sampler2D displacementTexture;
uniform sampler2D specularTexture;

void main()
{
	vec4 diffuseColor = vec4(texture(diffuseTexture1, vUV).xyz, 1.0f);
	vec4 normalValue = vec4(texture(normalTexture1, vUV).xyz, 1.0f);
	vec4 displacementValue = vec4(texture(displacementTexture, vUV).xyz, 1.0f);
	vec4 specularValue = vec4(texture(specularTexture, vUV).xyz, 1.0f);

	vec3 tangent = vTan;
	vec3 bitangent = vBitan;

	// Ambient component
	float ambientStrength = 0.3f;
	vec3 ambientComponent = ambientStrength * lightColor;

	// Test
	//fragmentColor = vec4(ambientComponent, 1.0f);
	
	// Diffuse component
	vec3 normal = normalize(vNorm);
	vec3 dir = normalize(-lightDir);
	float diffuse = max(dot(normal, dir), 0.0f);
	vec3 diffuseComponent = diffuse * lightColor;

	// Test 
	//fragmentColor = vec4(ambientComponent + diffuseComponent, 1.0f);
	//fragmentColor = vec4(normal, 1.0f);

	// Specular component
	vec3 viewDir = normalize(viewPos - vPosWorld);
	vec3 reflectDir = reflect(lightDir, normal);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specularComponent = specularStrength * specular * lightColor;

	// Test
	//fragmentColor = vec4(specularComponent, 1.0f);
	
	vec3 lightComponents = (ambientComponent + diffuseComponent + specularComponent);
	fragmentColor = vec4(lightComponents, 1.0f) * diffuseColor;	
}