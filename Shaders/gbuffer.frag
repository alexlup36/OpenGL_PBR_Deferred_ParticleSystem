#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gPBR;

in VS_OUT
{
    vec3 wsPosition;
    vec2 uv;
    mat3 tbn;
} fs_in;

uniform float dispMapScale;
uniform vec2 textureOffset;
uniform vec2 textureTile;
uniform vec3 viewPos;

uniform mat4 normalMat;
uniform sampler2D diffuseTexture1;
uniform sampler2D displacementTexture;
uniform sampler2D normalTexture1;
uniform sampler2D roughnessTexture;
uniform sampler2D metalnessTexture;
uniform sampler2D aoTexture;

// ----------------------------------------------------------------------------

vec2 parallaxMapping(vec2 texCoord, vec3 viewDirection)
{
	float height = texture(displacementTexture, texCoord).r;
	vec2 offset = viewDirection.xy * (height * dispMapScale);
	
	return texCoord - offset;
}

// ----------------------------------------------------------------------------

vec2 steepParallaxMapping(vec2 texCoord, vec3 viewDirection)
{
	const float minLayers = 8.0f;
	const float maxLayers = 32.0f;

	// Number of depth layers
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDirection)));
	// Calculate the size of each layer
	float layerDepth = 1.0f / numLayers;
	// Depth of the current layer
	float currentLayerDepth = 0.0f;
	// Calculate the amount to shift the tex coordinate for each layer
	vec2 p = viewDirection.xy * dispMapScale;
	vec2 texCoordDelta = p / numLayers;

	vec2 currentTexCoords = texCoord;
	float currentDepthMapValue = texture(displacementTexture, currentTexCoords).r;
	while (currentLayerDepth < currentDepthMapValue)
	{
		// Shift the texture coordinates along the p
		currentTexCoords -= texCoordDelta;
		// Get the depthmap value at the current texture coordinate
		currentDepthMapValue = texture(displacementTexture, currentTexCoords).r;
		// Get the depth of the next layer
		currentLayerDepth += layerDepth;
	}

	return currentTexCoords;
}

// ----------------------------------------------------------------------------

vec2 parallaxOcclusionMapping(vec2 texCoord, vec3 viewDirection)
{
	const float minLayers = 8.0f;
	const float maxLayers = 32.0f;

	// Number of depth layers
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDirection)));
	// Calculate the size of each layer
	float layerDepth = 1.0f / numLayers;
	// Depth of the current layer
	float currentLayerDepth = 0.0f;
	// Calculate the amount to shift the tex coordinate for each layer
	vec2 p = viewDirection.xy * dispMapScale;
	vec2 texCoordDelta = p / numLayers;

	vec2 currentTexCoords = texCoord;
	float currentDepthMapValue = texture(displacementTexture, currentTexCoords).r;
	while (currentLayerDepth < currentDepthMapValue)
	{
		// Shift the texture coordinates along the p
		currentTexCoords -= texCoordDelta;
		// Get the depthmap value at the current texture coordinate
		currentDepthMapValue = texture(displacementTexture, currentTexCoords).r;
		// Get the depth of the next layer
		currentLayerDepth += layerDepth;
	}

	// Get the texture coordinates before the collision
	vec2 prevTexCoords = currentTexCoords + texCoordDelta;

	// Linear interpolation between the depth before the collision and after the collision
	// The weight is represented by how far each depth is from the depth of the current layer
	float depthAfterCollision = currentDepthMapValue - currentLayerDepth;
	float depthBeforeCollision = texture(displacementTexture, prevTexCoords).r - currentLayerDepth + layerDepth;

	// Calculate the weight for the interpolation
	float weight = depthAfterCollision / (depthAfterCollision - depthBeforeCollision);

	// Interpolate the prev texture coordinates and the current texture coordinates based
	// on the weight calculated before
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0f - weight);

	return finalTexCoords;
}

// ----------------------------------------------------------------------------

void main()
{
	// Texture tiling
	vec2 texCoord = fs_in.uv * textureTile + textureOffset;
	vec2 texCoordParallax = vec2(0.0f);

	if (dispMapScale != 0.0f)
	{
		vec3 viewDirectionWS = normalize(viewPos - fs_in.wsPosition);

		// Apply parallax mapping

		// Normal parallax mapping
		//texCoordParallax = parallaxMapping(texCoord, viewDirectionWS);
		// Steep parallax mapping
		//texCoordParallax = steepParallaxMapping(texCoord, viewDirectionWS);
		// Parallax occlusion mapping
		texCoordParallax = parallaxOcclusionMapping(texCoord, viewDirectionWS);

		if (texCoordParallax.x > 1.0f || texCoordParallax.y > 1.0f || texCoordParallax.x < 0.0f || texCoordParallax.y < 0.0f)
			discard;
	}
	else
	{
		// No parallax mapping
		texCoordParallax = texCoord;
	}

	texCoordParallax = fs_in.uv;

	// Fragment position
	gPosition = fs_in.wsPosition;
	// Normal
	vec4 normal = texture(normalTexture1, texCoordParallax);
	gNormal = normalize(fs_in.tbn * normal.rgb);
	// Albedo
	gAlbedo = texture(diffuseTexture1, texCoordParallax);
	// PBR - contains rough, metal, ao
	float roughness = texture(roughnessTexture, texCoordParallax).r;
	float metalness = texture(metalnessTexture, texCoordParallax).r;
	float ao = texture(aoTexture, texCoordParallax).r;
	gPBR = vec3(roughness, metalness, ao);
}