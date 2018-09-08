#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include "Common.h"
#include <glm/glm.hpp>
#include "Light.h"

#define WHITE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define BLACK glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)

class LightData
{

public:
	static LightData& getInstance()
	{
		static LightData instance;
		return instance;
	}
	void initialize();

	PointLight pointLight1;
	DirectionalLight directionalLight1;

private:
	LightData() {}

public:
	LightData(LightData const&) = delete;
	void operator=(LightData const&) = delete;

};

#endif // LIGHTDATA_H