#version 330 core

// ----------------------------------------------------------------------------

#define MAX_DIR_LIGHTS 1
#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1

// ----------------------------------------------------------------------------

struct DirectionalLight
{
    vec3 direction;
	vec3 ambientComp;
	vec3 diffuseComp;
	vec3 specularComp;
};

struct PointLight
{
    vec3 position;
	vec3 attenuation;
	vec3 ambientComp;
	vec3 diffuseComp;
	vec3 specularComp;
};

struct SpotLight
{
	vec3 position;
	vec3 attenuation;
	vec3 ambientComp;
	vec3 diffuseComp;
	vec3 specularComp;
	vec3 direction;
	float exponent;
	float cutoff;
	float coscutoff;
};

struct Material
{
	vec3 ambientComp;
	vec3 diffuseComp;
	vec3 specularComp;
	float shineness;
};

// ----------------------------------------------------------------------------

// Samplers
uniform sampler2D diffuseTexture1;
uniform sampler2D normalTexture1;
uniform sampler2D displacementTexture;
uniform sampler2D specularTexture;

// Shadow map
uniform sampler2D depthMap;

// Light sources
uniform DirectionalLight dirLight[MAX_DIR_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];
uniform float specularStrength;

// Normal mapping
uniform float dispMapScale;
uniform float normalMapScale;

// Material
uniform Material material;

// HDR
uniform float gamma;

// Debug
uniform int displayMode;
const int DIFFUSE = 0x00;
const int NORMAL = 0x01;
const int NORMAL_TEX = 0x02;
const int DIRLIGHT_SHADING = 0x03;
const int POINTLIGHT_SHADING = 0x04;
const int FINAL = 0x05;

// Input
in vec2 texCoord;
in vec3 vertexW;
in mat3 TBNMatrix;
in vec3 normalW;
in vec3 viewPosTangent;
in vec3 fragmentPosTangent;

// Output
out vec4 color;

// ----------------------------------------------------------------------------

vec4 phongShading(vec3 normal, vec4 color, vec3 viewDirection)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalDiffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalSpecular = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < MAX_DIR_LIGHTS; i++)
	{
		// Calculate ambient component
		totalAmbient += dirLight[i].ambientComp * material.ambientComp;

		// Calculate diffuse component
		vec3 lightDir = normalize(TBNMatrix * normalize(dirLight[i].direction));
		float diffuse = max(dot(normal, lightDir), 0.0f);
		totalDiffuse += diffuse * dirLight[i].diffuseComp * material.diffuseComp;
	
		// Calculate specular component
		vec3 reflectionDirection = normalize(reflect(-lightDir, normal));
		float specular = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shineness);
		totalSpecular += specular * dirLight[i].specularComp * material.specularComp * specularStrength;
	}
	
	return vec4(totalAmbient + totalDiffuse, 1.0f) * color + vec4(totalSpecular, 1.0f);
}

// ----------------------------------------------------------------------------

vec4 blinnPhongShadingPoint(vec3 normal, vec4 color, vec3 viewDirection)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalDiffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalSpecular = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		// Calculate ambient component ---------------------------------------------------------------
		vec3 ambientComponent = pointLight[i].ambientComp * material.ambientComp;
		
		// Calculate diffuse component ---------------------------------------------------------------
		vec3 diffuseComponent, lightVector;
		vec3 lightPosTangent = TBNMatrix * pointLight[i].position;
		lightVector = (lightPosTangent - fragmentPosTangent);
		
		float distance = length(lightVector);
		vec3 lightDir = normalize(lightVector);
		float diffuse = max(dot(normal, lightDir), 0.0f);
		diffuseComponent = diffuse * pointLight[i].diffuseComp * material.diffuseComp;
		
		// Calculate specular component --------------------------------------------------------------
		vec3 halfWayDirection = normalize(lightDir + viewDirection);
		float specular = pow(max(dot(normal, halfWayDirection), 0.0f), material.shineness);
		vec3 specularComponent = specular * pointLight[i].specularComp * 
			material.specularComp;
		
		// Calculate the attenuation
		float attenuation = 1.0f / (pointLight[i].attenuation.z + pointLight[i].attenuation.y * distance + pointLight[i].attenuation.x * distance * distance);
		
		totalAmbient += ambientComponent * attenuation;
		totalDiffuse += diffuseComponent * attenuation;
		totalSpecular += specularComponent * attenuation * specularStrength;
	}

	return vec4(totalAmbient + totalDiffuse, 1.0f) * color + vec4(totalSpecular, 1.0f);
}

// ----------------------------------------------------------------------------

vec4 blinnPhongShading(vec3 normal, vec4 color, vec3 viewDirection)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalDiffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalSpecular = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < MAX_DIR_LIGHTS; i++)
	{
		// Transform light direction to tangent space


		// Calculate ambient component ---------------------------------------------------------------
		totalAmbient += dirLight[i].ambientComp * material.ambientComp;
	
		// Calculate diffuse component ---------------------------------------------------------------
		vec3 lightDir = -normalize(TBNMatrix * normalize(dirLight[i].direction));
		float diffuse = max(dot(normal, lightDir), 0.0f);
		totalDiffuse += diffuse * dirLight[i].diffuseComp * material.diffuseComp;
	
		// Calculate specular component --------------------------------------------------------------
		vec3 halfWayDirection = normalize(lightDir + viewDirection);
		float specular = pow(max(dot(normal, halfWayDirection), 0.0f), material.shineness);
		totalSpecular += specular * dirLight[i].specularComp * material.specularComp * specularStrength;
	}
	
	return vec4(totalAmbient + totalDiffuse, 1.0f) * color + vec4(totalSpecular, 1.0f);
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

// ----------------------------------------------------------------------------

void main()
{
	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Parallax displacement mapping
	vec3 viewDirectionTangent = normalize(viewPosTangent - fragmentPosTangent);
	
	// Normal parallax mapping
	//vec2 texCoordParallax = parallaxMapping(texCoord, viewDirectionTangent);
	// Steep parallax mapping
	//vec2 texCoordParallax = steepParallaxMapping(texCoord, viewDirectionTangent);
	// Parallax occlusion mapping
	vec2 texCoordParallax = parallaxOcclusionMapping(texCoord, viewDirectionTangent);
	
	if (texCoordParallax.x > 1.0f || texCoordParallax.y > 1.0f || texCoordParallax.x < 0.0f || texCoordParallax.y < 0.0f)
		discard;
	
	// Sample the diffuse and normal textures
	vec4 diffuseColor = texture(diffuseTexture1, texCoordParallax);
	// The normal is already in tangent space so we don't need to transform it using the TBN matrix
	vec3 normal = texture(normalTexture1, texCoordParallax).xyz;
	normal = normalMapScale * normalize(2.0f * normal - 1.0f);

	// Debug
	switch (displayMode)
	{
		case DIFFUSE:
			color = diffuseColor;
			break;
		case NORMAL:
			color = vec4(normalW, 1.0f);
			break;
		case NORMAL_TEX:
			color = vec4(normal, 1.0f);
			break;
		case DIRLIGHT_SHADING:
			color += blinnPhongShading(normal, diffuseColor, viewDirectionTangent);
			break;
		case POINTLIGHT_SHADING:
			color += blinnPhongShadingPoint(normal, diffuseColor, viewDirectionTangent);
			break;
		case FINAL:
		{
			//color += blinnPhongShading(normal, diffuseColor, viewDirectionTangent);
			color += blinnPhongShadingPoint(normal, diffuseColor, viewDirectionTangent);
			break;
		}
		default:
			color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	// Do gamma correction
	color.rgb = pow(color.rgb, vec3(1.0f / gamma));
	color.a = 1.0f;
}