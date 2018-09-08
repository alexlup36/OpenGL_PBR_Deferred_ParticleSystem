#include "MaterialData.h"

void MaterialData::initialize()
{
	mat1 = {};
	mat1.ambientComp = glm::vec3(0.6f, 0.3f, 0.3f);
	mat1.diffuseComp = glm::vec3(0.5f, 0.5f, 0.7f);
	mat1.specularComp = glm::vec3(1.0f, 1.0f, 1.0f);
	mat1.shineness = 2.0f;

	matPBR1 = {};
	matPBR1.albedo = glm::vec3(0.7f, 0.8f, 0.7f);
	matPBR1.ao = 0.5f;
	matPBR1.metallic = 0.4f;
	matPBR1.roughness = 0.7f;

	matTexPBR1 = {};
}
