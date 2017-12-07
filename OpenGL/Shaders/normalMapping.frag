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

// Light sources
uniform DirectionalLight dirLight[MAX_DIR_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];
uniform float specularStrength;

// Normal mapping
uniform float dispMapScale;

// Material
uniform Material material;

// HDR
uniform float gamma;

// Camera
uniform vec3 viewPos;

// Input
in vec2 texCoord;
in vec3 vertexW;
in mat3 TBNMatrix;
in vec3 normalW;

// Output
out vec4 color;

// ----------------------------------------------------------------------------

vec4 phongShading(vec3 normal, vec4 color)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalDiffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalSpecular = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < MAX_DIR_LIGHTS; i++)
	{
		// Calculate ambient component
		totalAmbient += dirLight[i].ambientComp * material.ambientComp;

		// Calculate diffuse component
		vec3 lightDir = normalize(dirLight[i].direction);
		float diffuse = max(dot(normal, lightDir), 0.0f);
		totalDiffuse += diffuse * dirLight[i].diffuseComp * material.diffuseComp;
	
		// Calculate specular component
		vec3 viewDirection = normalize(viewPos - vertexW);
		vec3 reflectionDirection = normalize(reflect(-lightDir, normal));
		float specular = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shineness);
		totalSpecular += specular * dirLight[i].specularComp * material.specularComp * specularStrength;
	}
	
	return vec4(totalAmbient + totalDiffuse, 1.0f) * color + vec4(totalSpecular, 1.0f);
}

// ----------------------------------------------------------------------------

vec4 blinnPhongShadingPoint(vec3 normal, vec4 color)
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
		lightVector = (pointLight[i].position - vertexW);
		
		float distance = length(lightVector);
		vec3 lightDir = normalize(lightVector);
		float diffuse = max(dot(normal, lightDir), 0.0f);
		diffuseComponent = diffuse * pointLight[i].diffuseComp * material.diffuseComp;
		
		// Calculate specular component --------------------------------------------------------------
		vec3 viewDirection = normalize(viewPos - vertexW);
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

vec4 blinnPhongShading(vec3 normal, vec4 color)
{
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalDiffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalSpecular = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < MAX_DIR_LIGHTS; i++)
	{
		// Calculate ambient component ---------------------------------------------------------------
		totalAmbient += dirLight[i].ambientComp * material.ambientComp;
	
		// Calculate diffuse component ---------------------------------------------------------------
		vec3 lightDir = normalize(-dirLight[i].direction);
		float diffuse = max(dot(normal, lightDir), 0.0f);
		totalDiffuse += diffuse * dirLight[i].diffuseComp * material.diffuseComp;
	
		// Calculate specular component --------------------------------------------------------------
		vec3 viewDirection = normalize(viewPos - vertexW);
		vec3 halfWayDirection = normalize(lightDir + viewDirection);
		float specular = pow(max(dot(normal, halfWayDirection), 0.0f), material.shineness);
		totalSpecular += specular * dirLight[i].specularComp * material.specularComp * specularStrength;
	}
	
	return vec4(totalAmbient + totalDiffuse, 1.0f) * color + vec4(totalSpecular, 1.0f);
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

// ----------------------------------------------------------------------------

void main()
{
	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Parallax displacement mapping
	vec3 viewDirectionTangent = normalize(TBNMatrix * (viewPos - vertexW));
	vec2 texCoordParallax = ParallaxMapping(texCoord, viewDirectionTangent);
	if (texCoordParallax.x > 1.0f || texCoordParallax.y > 1.0f || texCoordParallax.x < 0.0f || texCoordParallax.y < 0.0f)
	{
		discard;
	}
	
	// Sample the diffuse and normal textures
	vec4 diffuseColor = texture(diffuseTexture1, texCoordParallax);
	vec3 normal = normalize(TBNMatrix * (2.0f * texture(normalTexture1, texCoordParallax).xyz - 1.0f));
	
	// Calculate lighting using the Phong model texture
	color += blinnPhongShading(normal, diffuseColor);
	//color += blinnPhongShadingPoint(normal, diffuseColor);
}