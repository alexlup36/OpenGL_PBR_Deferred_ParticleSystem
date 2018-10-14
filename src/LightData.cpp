#include "LightData.h"

unsigned int DirectionalLight::m_dirLightCounter = 0;
unsigned int PointLight::m_pointLightCounter = 0;
unsigned int SpotLight::m_spotLightCounter = 0;

void LightData::initialize()
{
	// Reserve space to store light source information
	m_pointLights.reserve(MAX_POINT_LIGHTS);
	m_directionalLights.reserve(MAX_DIR_LIGHTS);
	m_spotLights.reserve(MAX_SPOT_LIGHTS);

	// Test point light source
	PointLight pointLight1 = {};
	pointLight1.ambientComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight1.diffuseComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight1.specularComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight1.attenuation = glm::vec3(0.0028f, 0.027f, 1.0f);
	pointLight1.position = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLight1.color = glm::vec3(1.0f, 1.0f, 1.0f);
	addPointLight(pointLight1);

	// Test directional light source
	DirectionalLight directionalLight1 = {};
	directionalLight1.ambientComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.diffuseComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.specularComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.direction = glm::vec3(1.0f, 0.0f, 0.0f);
	directionalLight1.color = glm::vec3(1.0f, 1.0f, 1.0f);
	addDirectionalLight(directionalLight1);
}
