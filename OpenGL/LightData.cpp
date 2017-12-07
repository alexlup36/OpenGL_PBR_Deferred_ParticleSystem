#include "LightData.h"

void LightData::initialize()
{
	pointLight1 = {};
	pointLight1.ambientComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight1.diffuseComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight1.specularComp = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight1.attenuation = glm::vec3(0.0028f, 0.027f, 1.0f);
	pointLight1.direction = glm::vec3(0.0f, 0.0f, 0.0f);
	pointLight1.color = glm::vec3(1.0f, 1.0f, 1.0f);

	directionalLight1 = {};
	directionalLight1.ambientComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.diffuseComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.specularComp = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight1.direction = glm::vec3(1.0f, 0.0f, 0.0f);
	directionalLight1.color = glm::vec3(1.0f, 1.0f, 1.0f);
}
