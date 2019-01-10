#version 330 core

// ------------------------------------------------------------------
// ------------------------------------------------------------------

#define PI 3.1415926535897932384626433832795
#define PI_2 1.57079632679489661923
#define PI_4 0.785398163397448309616

#define MAX_POINT_LIGHTS 20
#define MAX_DIR_LIGHTS 20
#define MAX_SPOT_LIGHTS 20

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// Struct definition

struct DirectionalLight
{
    vec3 direction;
	vec3 color;
	bool enabled;
};

struct PointLight
{
    vec3 position;
	vec3 attenuation;
	vec3 color;
	bool enabled;
};

struct SpotLight
{
	vec3 position;
	vec3 color;
	vec3 direction;
	float exponent;
	float cutoff;
	float coscutoff;
	bool enabled;
};

struct PBRMaterial
{
	vec3 color;
	float metallic;
	float roughness;
	float ao;
};

// ------------------------------------------------------------------
// ------------------------------------------------------------------

// Uniforms

// Samplers
uniform DirectionalLight dirLight[MAX_DIR_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

// Samplers
uniform sampler2D diffuseTexture1;
uniform sampler2D normalTexture1;
uniform sampler2D displacementTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D metalnessTexture;
uniform sampler2D aoTexture;

// Shadow mapping
uniform sampler2D shadowMap;

// Parallax mapping
uniform float dispMapScale;
uniform float normalMapScale;

// Texture
uniform vec2 textureOffset;
uniform vec2 textureTile;

// Debug
uniform int displayMode;
const int DIFFUSE = 0x00;
const int NORMAL = 0x01;
const int NORMAL_TEX = 0x02;
const int DIRLIGHT_SHADING = 0x03;
const int POINTLIGHT_SHADING = 0x04;
const int FINAL = 0x05;

// Gamma
uniform float gamma;

// ------------------------------------------------------------------

in VS_OUT
{
	vec2 texCoord;
	vec3 normalW;
	vec3 vertexW;
	mat3 TBNMatrix;
	vec3 viewPosTangent;
	vec3 fragmentPosTangent;
} fs_in;

// Store PBR properties from the PBR texture
PBRMaterial material;

// ------------------------------------------------------------------

out vec4 color;

// ------------------------------------------------------------------
// ------------------------------------------------------------------

// Normal distribution function - Trwobridge-Reitz GGX
float ndf_ggxtr(vec3 normal, vec3 halfway, float roughness)
{
	float roughness2 = roughness * roughness;
	float roughness4 = roughness2 * roughness2;
	float nDoth = max(dot(normal, halfway), 0.0f);
	float nDoth2 = nDoth * nDoth;

	float nom = roughness4;
	float denom = nDoth2 * (roughness4 - 1.0f) + 1.0f;
	denom = PI * denom * denom;

	return nom / denom;
}

float ndf_ggxtr2(vec3 normal, vec3 halfway, float roughness)
{
	float roughness2 = roughness * roughness;
	float nDoth = max(dot(normal, halfway), 0.0f);
	float nDoth2 = nDoth * nDoth;

	float nom = roughness2;
	float denom = nDoth2 * (roughness2 - 1.0f) + 1.0f;
	denom = PI * denom * denom;

	return nom / denom;
}

// ------------------------------------------------------------------

// Roughness remap function for dielectrics
float roughnessRemapDielectrics(float roughness)
{
	float r = roughness + 1.0f;
	return (r * r) / 8.0f;
}

// ------------------------------------------------------------------

// Roughness remap function for conductors
float roughnessRemapConductors(float roughness)
{
	return (roughness * roughness) / 2.0f;
}

// ------------------------------------------------------------------

// Geometry function - Smith's method using SchlickGGX
float gf_schlickggx(float dot, float remapRoughness)
{
	float nom = dot;
	float denom = dot * (1.0f - remapRoughness) + remapRoughness;

	return nom / denom;
}

// ------------------------------------------------------------------

// Smith's method to take into account both the light direction and the view direction
float gf_smith(vec3 normal, vec3 view, vec3 light, float remapRoughness)
{
	float nDotV = max(dot(normal, view), 0.0f);
	float nDotL = max(dot(normal, light), 0.0f);
	float schlickggx_ndotv = gf_schlickggx(nDotV, remapRoughness);
	float schilckggx_ndotl = gf_schlickggx(nDotL, remapRoughness);

	return schlickggx_ndotv * schilckggx_ndotl;
}

// ------------------------------------------------------------------

// Fresnel approximation
vec3 calculateBaseReflectivity(vec3 surfaceColor, float metalic)
{
	// Surface reflection at zero incidence
	vec3 f0 = vec3(0.04f);
	return mix(surfaceColor, surfaceColor, metalic);
}

// ------------------------------------------------------------------

// Schilck's approximation
vec3 fresnel_schilck(float cosTheta, vec3 baseReflectivity)
{
	return baseReflectivity + (1.0f - baseReflectivity) * pow(1.0f - cosTheta, 5.0f);
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------

// Point light attenuation
float calculateAttenuationDistance(vec3 vertexPosW, vec3 lightPosW)
{
	float distance = length(lightPosW - vertexPosW);
	return 1.0f / (distance * distance);
}

// ------------------------------------------------------------------

// Point light attenuation
float calculateAttenuationQuadratic(vec3 vertexPosW, vec3 lightPosW, vec3 attenuation)
{
	float distance = length(lightPosW - vertexPosW);
	return 1.0f / (attenuation.z + attenuation.y * distance + attenuation.x * (distance * distance));
}

// ------------------------------------------------------------------

vec4 pbrShadingPoint(vec3 normal, vec3 viewDirection)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalIrradiance = vec3(0.0f, 0.0f, 0.0f);

	// View direction
	vec3 v = viewDirection;
	// Fragment normal direction
	vec3 n = normal;

	// Calculate f0 - base reflectivity
	vec3 f0 = calculateBaseReflectivity(material.color, material.metallic);

	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (pointLight[i].enabled == false) continue;

		// Calculate light position in tangent space
		vec3 lightPosTangent = fs_in.TBNMatrix * pointLight[i].position;

		// Point light direction
		vec3 l = normalize(lightPosTangent - fs_in.fragmentPosTangent);
		// Calculate attenuation
		float attenuation = calculateAttenuationQuadratic(fs_in.fragmentPosTangent, lightPosTangent, pointLight[i].attenuation);
		// Cos theta
		float cosTheta = max(dot(n, l), 0.0f);
		// Calculate radiance
		vec3 radiance = pointLight[i].color * attenuation * cosTheta;
		// Calculate the halfway vector
		vec3 h = normalize(v + l);

		// Calculate Schlick's approximation for the Fresnel factor
		vec3 F = fresnel_schilck(max(dot(h, v), 0.0f), f0);
		// Calculate the geometry function
		float GF = gf_smith(n, v, l, roughnessRemapDielectrics(material.roughness));
		// Calculate the normal distribution function
		float NDF = ndf_ggxtr(n, h, material.roughness);
		// Calculate BRDF Cook-Torrance
		vec3 nom = NDF * GF * F;
		float denom = 4.0f * max(dot(n, v), 0.0f) * cosTheta + 0.001f;
		vec3 brdf = nom / denom;

		// Calculate refracted/reflected incoming light ratio
		vec3 ks = F;
		vec3 kd = vec3(1.0f) - ks;
		// Cancel kd if the material is metallic
		kd *= (1.0f - material.metallic);

		// Calculate irradiance
		vec3 lambert = material.color / PI;
		totalIrradiance += (kd * lambert + brdf) * radiance;
	}

	// Calculate ambient lighting
	totalAmbient += vec3(0.0f) * material.color * material.ao;

	return vec4(totalAmbient + totalIrradiance, 1.0f);
}

// ------------------------------------------------------------------

vec4 pbrShadingDir(vec3 normal, vec3 viewDirection)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalIrradiance = vec3(0.0f, 0.0f, 0.0f);

	// View direction
	vec3 v = viewDirection;
	//return vec4(v, 1.0f);

	// Fragment normal direction
	vec3 n = normal;
	//return vec4(n, 1.0f);

	// Calculate f0 - base reflectivity
	vec3 f0 = calculateBaseReflectivity(material.color, material.metallic);
	//return vec4(f0, 1.0f);

	for (int i = 0; i < MAX_DIR_LIGHTS; ++i)
	{
		if (dirLight[i].enabled == false) continue;

		// Directional light direction in tangent space
		vec3 l = -normalize(fs_in.TBNMatrix * normalize(dirLight[i].direction));
		//return vec4(l, 1.0f);
		
		// Cos theta
		float cosTheta = max(dot(n, l), 0.0f);
		//return vec4(cosTheta, 0.0f, 0.0f, 1.0f);

		// Calculate radiance
		vec3 radiance = dirLight[i].color * cosTheta;
		//return vec4(radiance, 1.0f);

		// Calculate the halfway vector
		vec3 h = normalize(v + l);
		//return vec4(h, 1.0f);

		// Calculate Schlick's approximation for the Fresnel factor
		vec3 F = fresnel_schilck(max(dot(h, v), 0.0f), f0);
		//return vec4(F, 1.0f);

		// Calculate the geometry function
		float GF = gf_smith(n, v, l, roughnessRemapDielectrics(material.roughness));
		//return vec4(GF, 0.0f, 0.0f, 1.0f);

		// Calculate the normal distribution function
		float NDF = ndf_ggxtr(n, h, material.roughness);
		//float NDF = ndf_ggxtr(n, h, 0.8f); // roughness^4
		//float NDF = ndf_ggxtr2(n, h, 0.8f); // roughness^2
		//return vec4(NDF, 0.0f, 0.0f, 1.0f);

		// Calculate BRDF Cook-Torrance
		vec3 nom = NDF * GF * F;
		float denom = 4.0f * max(dot(n, v), 0.0f) * cosTheta + 0.001f;
		vec3 brdf = nom / denom;
		//return vec4(brdf, 1.0f);

		// Calculate refracted/reflected incoming light ratio
		vec3 ks = F;
		vec3 kd = vec3(1.0f) - ks;
		// Cancel kd if the material is metallic
		kd *= (1.0f - material.metallic);

		// Calculate irradiance
		vec3 lambert = material.color / PI;
		totalIrradiance += (kd * lambert + brdf) * radiance;

		// Calculate ambient lighting
		totalAmbient += vec3(0.0f) * material.color * material.ao;
	}

	//return vec4(totalAmbient, 1.0f);
	return vec4(totalAmbient + totalIrradiance, 1.0f);
}

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

// ------------------------------------------------------------------

void main()
{
	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// --------------------------------------
	// Parallax displacement mapping
	vec3 viewDirectionTangent = normalize(fs_in.viewPosTangent - fs_in.fragmentPosTangent);
	
	// Texture tiling
	vec2 texCoord = fs_in.texCoord * textureTile + textureOffset;
	vec2 texCoordParallax = vec2(0.0f);

	if (dispMapScale != 0.0f)
	{
		// Apply parallax mapping

		// Normal parallax mapping
		//texCoordParallax = parallaxMapping(texCoord, viewDirectionTangent);
		// Steep parallax mapping
		//texCoordParallax = steepParallaxMapping(texCoord, viewDirectionTangent);
		// Parallax occlusion mapping
		texCoordParallax = parallaxOcclusionMapping(texCoord, viewDirectionTangent);

		if (texCoordParallax.x > 1.0f || texCoordParallax.y > 1.0f || texCoordParallax.x < 0.0f || texCoordParallax.y < 0.0f)
			discard;
	}
	else
	{
		// No parallax mapping
		texCoordParallax = texCoord;
	}
	
	// --------------------------------------

	// Get PBR material
	// Sample the diffuse texture
	material.color = texture(diffuseTexture1, texCoordParallax).xyz;
	// The normal is already in tangent space so we don't need to transform it using the TBN matrix
	vec3 normal = texture(normalTexture1, texCoordParallax).xyz;
	normal = normalMapScale * normalize(2.0f * normal - 1.0f);
	// Sample the PBR textures	
	// PBR material initialize
	material.roughness = texture(roughnessTexture, texCoordParallax).x;
	material.metallic = texture(metalnessTexture, texCoordParallax).x;
	material.ao = texture(aoTexture, texCoordParallax).x;

	// --------------------------------------

	// Shadow mapping
	// Sample depth map
	vec3 depth = texture(shadowMap, texCoordParallax).xyz;

	// --------------------------------------

	// Calculate lighting
	// Debug
	switch (displayMode)
	{
		case DIFFUSE:
			color = vec4(material.color, 1.0f);
			break;
		case NORMAL:
			color = vec4(fs_in.normalW, 1.0f);
			break;
		case NORMAL_TEX:
			color = vec4(normal, 1.0f);
			break;
		case DIRLIGHT_SHADING:
			color += pbrShadingDir(normal, viewDirectionTangent);
			break;
		case POINTLIGHT_SHADING:
			color += pbrShadingPoint(normal, viewDirectionTangent);
			break;
		case FINAL:
		{
			color += pbrShadingDir(normal, viewDirectionTangent);
			color += pbrShadingPoint(normal, viewDirectionTangent);
			break;
		}
		default:
			color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	// --------------------------------------

	// Do gamma correction
	color.rgb = pow(color.rgb, vec3(1.0f / gamma));
	color.a = 1.0f;

	// --------------------------------------
}

// ------------------------------------------------------------------