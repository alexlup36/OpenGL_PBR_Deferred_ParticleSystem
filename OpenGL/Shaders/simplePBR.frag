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

// Normal distribution function - Trwobridge-Reitz GGX
float ndf_ggxtr(vec3 normal, vec3 halfway, float roughness)
{
	float roughness2 = roughness * roughness;
	float roughness4 = roughness2 * roughness2;
	float nDoth = dot(normal, halfway);
	float nDoth2 = nDoth * nDoth;

	float nom = roughness4;
	float denom = nDoth2 * (roughness4 - 1.0f) + 1.0f;
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
	return mix(f0, surfaceColor, metalic);
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
float calculateAttenuationDistance(vec3 vertexW, vec3 lightPosW)
{
	float distance = length(lightPosW - vertexW);
	return 1.0f / (distance * distance);
}

// ------------------------------------------------------------------

// Point light attenuation
float calculateAttenuationQuadratic(vec3 vertexW, vec3 lightPosW, vec3 attenuation)
{
	float distance = length(lightPosW - vertexW);
	return 1.0f / (attenuation.z + attenuation.y * distance + attenuation.x * (distance * distance));
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------

// Uniforms

// Light sources
uniform DirectionalLight dirLight[MAX_DIR_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

// Material
uniform PBRMaterial material;

// Camera
uniform vec3 viewPos;

// Normal mapping
uniform float dispMapScale;

// Shadow map
uniform sampler2D depthTexture;

// Gamma
uniform float gamma;

// ------------------------------------------------------------------

in VS_OUT
{
	vec3 normalW;
	vec3 vertexW;
	vec2 vertexUV;
} fs_in;

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

	return vec4(n, 1.0f);

	// Calculate f0 - base reflectivity
	vec3 f0 = calculateBaseReflectivity(material.color, material.metallic);

	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		// Point/directional light direction
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
		// Calculate ambient lighting
		totalAmbient += vec3(0.03f) * material.color * material.ao;
	}

	return vec4(totalAmbient + totalIrradiance, 1.0f);
}

// ------------------------------------------------------------------

vec4 pbrShadingDir(vec3 normal)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalIrradiance = vec3(0.0f, 0.0f, 0.0f);

	// View direction
	vec3 v = normalize(viewPos - fs_in.vertexW);
	// Fragment normal direction
	vec3 n = normalize(normal);

	// Calculate f0 - base reflectivity
	vec3 f0 = calculateBaseReflectivity(material.color, material.metallic);

	for (int i = 0; i < MAX_DIR_LIGHTS; ++i)
	{
		// Directional light direction
		vec3 l = -normalize(dirLight[i].direction);
		// Cos theta
		float cosTheta = max(dot(n, l), 0.0f);
		// Calculate radiance
		vec3 radiance = dirLight[i].color * cosTheta;
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

		// Calculate ambient lighting
		totalAmbient += vec3(0.03f) * material.color * material.ao;
	}

	return vec4(totalAmbient + totalIrradiance, 1.0f);
}

// ------------------------------------------------------------------

void main()
{
	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Sample depth map
	vec3 depth = texture(depthTexture, fs_in.vertexUV).xyz;

	// Calculate lighting
	vec3 normal = normalize(fs_in.normalW);
	//color += pbrShadingDir(normal);
	color += pbrShadingPoint(normal);

	// Do gamma correction
	color.rgb = pow(color.rgb, vec3(1.0f / gamma));
	color.a = 1.0f;
}

// ------------------------------------------------------------------