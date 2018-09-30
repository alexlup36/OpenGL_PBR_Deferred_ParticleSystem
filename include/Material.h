#ifndef MATERIAL_H
#define MATERIAL_H

// ----------------------------------------------------------------------------

#include "Common.h"
#include <glm/glm.hpp>
#include "Texture2D.h"

// ----------------------------------------------------------------------------

struct MaterialColor
{
	glm::vec3 ambientComp;
	glm::vec3 diffuseComp;
	glm::vec3 specularComp;
	float shineness;

	MaterialColor()
	{
		ambientComp = glm::vec3(1.0f);
		diffuseComp = glm::vec3(1.0f);
		specularComp = glm::vec3(1.0f);
		shineness = 10.0f;
	}

	MaterialColor(const glm::vec3& ambient,
		const glm::vec3 diffuse,
		const glm::vec3 specular,
		const float shinenessVal)
	{
		ambientComp = ambient;
		diffuseComp = diffuse;
		specularComp = specular;
		shineness = shinenessVal;
	}
};

struct MaterialColorPBR
{
	glm::vec3 albedo;
	float metallic;
	float roughness;
	float ao;

	MaterialColorPBR()
	{
		albedo = glm::vec3(1.0f, 0.0f, 0.0f);
		metallic = 1.0f;
		roughness = 0.1f;
		ao = 0.5f;
	}

	MaterialColorPBR(const glm::vec3& albedo,
		float metallic,
		float roughness,
		float ao)
	{
		this->albedo = albedo;
		this->metallic = metallic;
		this->roughness = roughness;
		this->ao = ao;
	}
};

struct MaterialTexturePBR
{
	Texture2D* albedo;
	Texture2D* normal;
	Texture2D* displacement;
	Texture2D* metallic;
	Texture2D* roughness;
	Texture2D* ao;

	MaterialTexturePBR()
	{
		albedo = nullptr;
		normal = nullptr;
		displacement = nullptr;
		metallic = nullptr;
		roughness = nullptr;
		ao = nullptr;
	}

	MaterialTexturePBR(Texture2D* albedo,
		Texture2D* normal,
		Texture2D* displacement,
		Texture2D* metallic,
		Texture2D* roughness,
		Texture2D* ao)
	{
		this->albedo = albedo;
		this->normal = normal;
		this->displacement = displacement;
		this->metallic = metallic;
		this->roughness = roughness;
		this->ao = ao;
	}

	void bindTextures(GLuint program)
	{
		if (albedo) albedo->bind(program);
		//if (normal) normal->bind(program);
		//if (displacement) displacement->bind(program);
		//if (metallic) metallic->bind(program);
		//if (roughness) roughness->bind(program);
		//if (ao) ao->bind(program);
	}
};

// ----------------------------------------------------------------------------

#endif // MATERIAL_H