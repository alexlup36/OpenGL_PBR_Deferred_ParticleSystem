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
	PointLight pointLight0 = {};
	pointLight0.ambientComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight0.diffuseComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight0.specularComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight0.attenuation = glm::vec3(0.0028f, 0.027f, 1.0f);
	pointLight0.position = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLight0.color = glm::vec3(1.0f, 1.0f, 1.0f);
	addPointLight(pointLight0);

	// Test directional light source
	DirectionalLight directionalLight0 = {};
	directionalLight0.ambientComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight0.diffuseComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight0.specularComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight0.direction = glm::vec3(1.0f, 0.0f, 0.0f);
	directionalLight0.color = glm::vec3(1.0f, 1.0f, 1.0f);
	addDirectionalLight(directionalLight0);

	DirectionalLight directionalLight1 = {};
	directionalLight1.ambientComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.diffuseComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.specularComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.direction = glm::vec3(1.0f, 0.0f, 0.0f);
	directionalLight1.color = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.pbrLight = false;
	addDirectionalLight(directionalLight1);
}
