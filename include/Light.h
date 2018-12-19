#ifndef LIGHT_H
#define LIGHT_H

// ----------------------------------------------------------------------------

#include "Common.h"
#include <glm/glm.hpp>
#include <sstream>

// ----------------------------------------------------------------------------

enum class LightSourceType
{
	Point,
	Directional,
	Spot,
};

// ----------------------------------------------------------------------------

struct BaseLight
{
	glm::vec3 color;
	glm::vec3 ambientComp;
	glm::vec3 diffuseComp;
	glm::vec3 specularComp;
	unsigned int id;
	LightSourceType lightSourceType;
	bool enabled = true;
	bool pbrLight = true;

	BaseLight()
	{
		color = glm::vec3(1.0f);
		ambientComp = glm::vec3(1.0f);
		diffuseComp = glm::vec3(1.0f);
		specularComp = glm::vec3(1.0f);
	}

	BaseLight(const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular)
	{
		ambientComp = ambient;
		diffuseComp = diffuse;
		specularComp = specular;	
	}

	virtual const std::string getId() const = 0;
};

// ----------------------------------------------------------------------------

struct DirectionalLight : public BaseLight
{
	glm::vec3 direction;

	DirectionalLight() 
		: BaseLight()
	{
		direction = glm::vec3(0.0f);
		ambientComp = glm::vec3(1.0f);
		diffuseComp = glm::vec3(1.0f);
		specularComp = glm::vec3(1.0f);
		color = glm::vec3(1.0f);
		lightSourceType = LightSourceType::Directional;
		id = m_dirLightCounter++;
	}

	DirectionalLight(const glm::vec3& dir,
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular)
		: BaseLight(ambient, diffuse, specular)
	{
		direction = dir;
		color = glm::vec3(0.0f);
		lightSourceType = LightSourceType::Directional;
		id = m_dirLightCounter++;
	}

	DirectionalLight(const glm::vec3& dir,
		const glm::vec3& color)
		: DirectionalLight()
	{
		direction = dir;
		this->color = color;
	}

	const std::string getId() const override
	{
		std::ostringstream stream;
		stream << "DirectionalLight" << id;
		return stream.str();
	}

	static unsigned int m_dirLightCounter;
};

// -------------------------------------------------------------------------

struct PointLight : public BaseLight
{
	glm::vec3 position;
	glm::vec3 attenuation;
	unsigned int id;

	PointLight()
		: BaseLight()
	{
		position = glm::vec3(0.0f);
		attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
		lightSourceType = LightSourceType::Point;
		id = m_pointLightCounter++;
	}

	PointLight(const glm::vec3& pos,
		const glm::vec3& col,
		const glm::vec3& att)
		: PointLight()
	{
		position = pos;
		color = col;
		attenuation = att;
	}

	PointLight(const glm::vec3& pos,
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		const glm::vec3& att)
		: BaseLight(ambient, diffuse, specular)
	{
		position = pos;
		attenuation = att;
		lightSourceType = LightSourceType::Point;
		id = m_pointLightCounter++;
	}

	const std::string getId() const override
	{
		std::ostringstream stream;
		stream << "PointLight" << id;
		return stream.str().c_str();
	}

	static unsigned int m_pointLightCounter;
};

// -------------------------------------------------------------------------

struct SpotLight : public BaseLight
{
	float exponent;
	float cutoff;
	float coscutoff;
	glm::vec3 position;
	glm::vec3 direction;
	unsigned int id;

	SpotLight()
		: BaseLight()
	{
		exponent = 0.0f;
		cutoff = 0.0f;
		coscutoff = 0.0f;
		position = glm::vec3(0.0f);
		direction = glm::vec3(0.0f);
		lightSourceType = LightSourceType::Spot;
		id = m_spotLightCounter++;
	}

	SpotLight(const glm::vec3& pos,
		const glm::vec3& col,
		const glm::vec3& dir,
		const float exponentVal,
		const float cutoffVal,
		const float coscutoffVal)
		: SpotLight()
	{
		position = pos;
		color = col;
		direction = dir;		
		exponent = exponentVal;
		cutoff = cutoffVal;
		coscutoff = coscutoffVal;
	}

	SpotLight(const glm::vec3& pos,
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		const glm::vec3& dir,
		const float exponentVal,
		const float cutoffVal,
		const float coscutoffVal)
		: BaseLight(ambient, diffuse, specular)
	{
		position = pos;
		direction = dir;
		exponent = exponentVal;
		cutoff = cutoffVal;
		coscutoff = coscutoffVal;
		lightSourceType = LightSourceType::Spot;
		id = m_spotLightCounter++;
	}

	const std::string getId() const override
	{
		std::ostringstream stream;
		stream << "SpotLight" << id;
		return stream.str().c_str();
	}

	static unsigned int m_spotLightCounter;
};

// ----------------------------------------------------------------------------

#endif // LIGHT_H