#version 330 core

// ------------------------------------------------------------------
// ------------------------------------------------------------------

#define PI 3.1415926535897932384626433832795
#define PI_2 1.57079632679489661923
#define PI_4 0.785398163397448309616

#define MAX_POINT_LIGHTS 1
#define MAX_DIR_LIGHTS 1
#define MAX_SPOT_LIGHTS 1

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// Struct definition

struct DirectionalLight
{
    vec3 direction;
	vec3 color;
};

struct PointLight
{
    vec3 position;
	vec3 attenuation;
	vec3 color;
};

struct SpotLight
{
	vec3 position;
	vec3 attenuation;
	vec3 color;
	vec3 direction;
	float exponent;
	float cutoff;
	float coscutoff;
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

// Camera
uniform vec3 viewPos;

// Samplers
uniform sampler2D diffuseTexture1;
uniform sampler2D normalTexture1;
uniform sampler2D displacementTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D metalnessTexture;
uniform sampler2D aoTexture;

uniform sampler2D depthMap;

uniform bool isMetal;

// Parallax mapping
uniform float dispMapScale;

// Gamma
uniform float gamma;

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


// ----------------------------------------------------------------------------

vec2 ParallaxMapping(vec2 texCoord, vec3 viewDirection)
{
	// Number of depth layers
	const float minLayerCount = 8;
	const float maxLayerCount = 32;
	float layerCount = mix(maxLayerCount, minLayerCount, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDirection)));
	// Calculate the size of each layer
	float layerDepth = 1.0f / layerCount;
	// Depth of the current layer
	float currentLayerDepth = 0.0f;
	// Texture offset for each depth layer
	vec2 p = viewDirection.xy * dispMapScale;
	vec2 deltaTexCoords = p / layerCount;
	// Set initial values
	vec2 currentTexCoord = texCoord;
	float currentDepthMapValue = texture(displacementTexture, currentTexCoord).r;
	
	while (currentLayerDepth < currentDepthMapValue)
	{
		// Adjust the texture coordinates for the current depth values
		currentTexCoord -= deltaTexCoords;
		
		// Get the depth map value at the current texture coordinate
		currentDepthMapValue = texture(displacementTexture, currentTexCoord).r;
	
		// Increment the current layer depth
		currentLayerDepth += layerDepth;
	}
	
	// Get the previous tex coordinate
	vec2 prevTexCoord = currentTexCoord + deltaTexCoords;
	// Get the depth value after and before the collision
	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(displacementTexture, prevTexCoord).r - currentLayerDepth + layerDepth;
	
	// Do the interpolation
	float weight = afterDepth / (afterDepth - beforeDepth);
	
	return prevTexCoord * weight + currentTexCoord * (1.0f - weight);
}

// ------------------------------------------------------------------

in VS_OUT
{
	vec2 texCoord;
	vec3 normalW;
	vec3 vertexW;
	mat3 TBNMatrix;
} fs_in;

// Store PBR properties from the PBR texture
PBRMaterial material;

// ------------------------------------------------------------------

out vec4 color;

// ------------------------------------------------------------------

vec4 pbrShadingPoint(vec3 normal)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalIrradiance = vec3(0.0f, 0.0f, 0.0f);

	// View direction
	vec3 v = normalize(viewPos - fs_in.vertexW);
	// Fragment normal direction
	vec3 n = normalize(normal);

	// Calculate f0 - base reflectivity
	vec3 f0 = calculateBaseReflectivity(material.color, material.metallic);

	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		// Point light direction
		vec3 l = normalize(pointLight[i].position - fs_in.vertexW);
		// Calculate attenuation
		float attenuation = calculateAttenuationQuadratic(fs_in.vertexW, pointLight[i].position, pointLight[i].attenuation);
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
	totalAmbient += vec3(0.03f) * material.color * material.ao;

	return vec4(totalAmbient + totalIrradiance, 1.0f);
}

// ------------------------------------------------------------------

vec4 pbrShadingDir(vec3 normal)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalIrradiance = vec3(0.0f, 0.0f, 0.0f);

	// View direction
	vec3 v = normalize(viewPos - fs_in.vertexW);
	//return vec4(v, 1.0f);

	// Fragment normal direction
	vec3 n = normalize(normal);
	//return vec4(n, 1.0f);

	// Calculate f0 - base reflectivity
	vec3 f0 = calculateBaseReflectivity(material.color, material.metallic);
	//return vec4(f0, 1.0f);

	for (int i = 0; i < MAX_DIR_LIGHTS; ++i)
	{
		// Directional light direction
		vec3 l = -normalize(dirLight[i].direction);
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
		totalAmbient += vec3(0.03f) * material.color * material.ao;
	}

	//return vec4(totalAmbient, 1.0f);
	return vec4(totalAmbient + totalIrradiance, 1.0f);
}

// ------------------------------------------------------------------

void main()
{
	vec2 texCoord = fs_in.texCoord;

	if (dispMapScale != -1.0f)
	{
		// Parallax displacement mapping
		vec3 viewDirectionTangent = normalize(fs_in.TBNMatrix * (viewPos - fs_in.vertexW));
		vec2 texCoordParallax = ParallaxMapping(fs_in.texCoord, viewDirectionTangent);
		if (texCoordParallax.x > 1.0f || texCoordParallax.y > 1.0f || texCoordParallax.x < 0.0f || texCoordParallax.y < 0.0f)
		{
			discard;
		}
		texCoord = texCoordParallax;
	}

	// Sample the diffuse texture
	material.color = texture(diffuseTexture1, texCoord).xyz;
	// Sample the normal texture
	vec3 n = normalize(fs_in.TBNMatrix * (2.0f * texture(normalTexture1, texCoord).xyz - 1.0f));
	// Sample the PBR textures	
	// PBR material initialize
	material.roughness = texture(roughnessTexture, texCoord).x;
	material.metallic = texture(metalnessTexture, texCoord).x;
	material.ao = texture(aoTexture, texCoord).x;

	//color = vec4(material.roughness, 0.0f, 0.0f, 1.0f);
	//color = vec4(material.metallic, 0.0f, 0.0f, 1.0f);
	//color = vec4(material.color, 1.0f);
	//color = vec4(material.ao, 0.0f, 0.0f, 1.0f);
	//return;

	// Sample depth map
	vec3 depth = texture(depthMap, texCoord).xyz;

	// Default
	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Calculate lighting
	color += pbrShadingDir(n);
	color += pbrShadingPoint(n);

	// Do gamma correction
	color.rgb = pow(color.rgb, vec3(1.0f / gamma));
	color.a = 1.0f;
}

// ------------------------------------------------------------------