#ifndef LIGHT_H
#define LIGHT_H

// ----------------------------------------------------------------------------

#include "Common.h"
#include <glm/glm.hpp>

// ----------------------------------------------------------------------------

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 ambientComp;
	glm::vec3 diffuseComp;
	glm::vec3 specularComp;
	glm::vec3 color;

	DirectionalLight()
	{
		direction = glm::vec3(0.0f);
		ambientComp = glm::vec3(1.0f);
		diffuseComp = glm::vec3(1.0f);
		specularComp = glm::vec3(1.0f);
		color = glm::vec3(1.0f);
	}

	DirectionalLight(const glm::vec3& dir,
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular)
	{
		direction = dir;
		ambientComp = ambient;
		diffuseComp = diffuse;
		specularComp = specular;
		color = glm::vec3(0.0f);
	}

	DirectionalLight(const glm::vec3& dir,
		const glm::vec3& color)
	{
		direction = dir;
		ambientComp = glm::vec3(0.0f);
		diffuseComp = glm::vec3(0.0f);
		specularComp = glm::vec3(0.0f);
		this->color = color;
	}
};

// -------------------------------------------------------------------------

struct PointLight : public DirectionalLight
{
	glm::vec3 attenuation;

	PointLight()
		: DirectionalLight()
	{
		attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	PointLight(const glm::vec3& pos,
		const glm::vec3& col,
		const glm::vec3& att)
		: DirectionalLight(pos, col)
	{
		attenuation = att;
	}

	PointLight(const glm::vec3& pos,
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		const glm::vec3& att)
		: DirectionalLight(pos, ambient, diffuse, specular)
	{
		attenuation = att;
	}
};

// -------------------------------------------------------------------------

struct SpotLight : public PointLight
{
	float exponent;
	float cutoff;
	float coscutoff;
	glm::vec3 position;

	SpotLight()
		: PointLight()
	{
		position = glm::vec3(0.0f);
		exponent = 0.0f;
		cutoff = 0.0f;
		coscutoff = 0.0f;
	}

	SpotLight(const glm::vec3& pos,
		const glm::vec3& col,
		const glm::vec3& att,
		const glm::vec3& dir,
		const float exponentVal,
		const float cutoffVal,
		const float coscutoffVal)
		: PointLight(dir, col, att)
	{
		exponent = exponentVal;
		cutoff = cutoffVal;
		coscutoff = coscutoffVal;
	}

	SpotLight(const glm::vec3& pos,
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		const glm::vec3& att,
		const glm::vec3& dir,
		const float exponentVal,
		const float cutoffVal,
		const float coscutoffVal)
		: PointLight(dir, ambient, diffuse, specular, att)
	{
		exponent = exponentVal;
		cutoff = cutoffVal;
		coscutoff = coscutoffVal;
	}
};

// ----------------------------------------------------------------------------

#endif // LIGHT_H