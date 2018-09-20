#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gTangent;
layout (location = 2) out vec3 gNormal;
layout (location = 3) out vec4 gAlbedo;
layout (location = 4) out vec4 gPBR;

in vec2 uv;
in vec3 wsPosition;
in vec3 wsTangent;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D displacementTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D metalnessTexture;
uniform sampler2D aoTexture;

void main()
{
	// Fragment position
	gPosition = wsPosition;
	// Fragment tangent
	gTangent = wsTangent;
	// Fragment normal
	gNormal = normalize(texture(normalTexture, uv).rgb);
	// Albedo
	gAlbedo = texture(diffuseTexture, uv);
	// PBR - contains disp, rough, metal, ao
	float displacement = texture(displacementTexture, uv).r;
	float roughness = texture(roughnessTexture, uv).r;
	float metalness = texture(metalnessTexture, uv).r;
	float ao = texture(aoTexture, uv).r;
	gPBR = vec4(displacement, roughness, metalness, ao);
}