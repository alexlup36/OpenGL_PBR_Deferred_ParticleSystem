#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include "Common.h"
#include <glm/glm.hpp>
#include "Light.h"

#include <vector>
#include <map>

#define WHITE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define BLACK glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)

const unsigned int MAX_POINT_LIGHTS = 20;
const unsigned int MAX_DIR_LIGHTS = 20;
const unsigned int MAX_SPOT_LIGHTS = 20;

class LightData
{

public:
	static LightData& getInstance()
	{
		static LightData instance;
		return instance;
	}
	void initialize();

	// Add light sources
	inline void addPointLight(const PointLight &pointLight) 
	{
		assert(m_pointLights.size() < MAX_POINT_LIGHTS);
		m_pointLights.push_back(pointLight);
		m_globalLights[m_globalIndex++] = &m_pointLights.back();
	}
	inline void addDirectionalLight(const DirectionalLight &directionalLight) 
	{
		assert(m_directionalLights.size() < MAX_DIR_LIGHTS);
		m_directionalLights.push_back(directionalLight);
		m_globalLights[m_globalIndex++] = &m_directionalLights.back();
	}
	inline void addSpotLight(const SpotLight &spotLight) 
	{
		assert(m_spotLights.size() < MAX_SPOT_LIGHTS);
		m_spotLights.push_back(spotLight);
		m_globalLights[m_globalIndex++] = &m_spotLights.back();
	}

	// Get light sources
	inline PointLight &pointLight(int index)
	{
		assert(index < m_pointLights.size());
		return m_pointLights[index];
	}
	inline DirectionalLight &directionalLight(int index)
	{
		assert(index < m_directionalLights.size());
		return m_directionalLights[index];
	}
	inline SpotLight &spotLight(int index)
	{
		assert(index < m_spotLights.size());
		return m_spotLights[index];
	}
	inline BaseLight* getLight(int index)
	{
		assert(index < m_globalLights.size());
		return m_globalLights[index];
	}

	std::vector<std::string> getIds()
	{
		std::vector<std::string> ids;
		for (auto &lightSource : m_globalLights)
			ids.push_back(lightSource.second->getId());
		return ids;
	}

	inline int lightCount() const { return m_pointLights.size() + m_directionalLights.size() + m_spotLights.size(); }
	inline int pointLightCount() const { return m_pointLights.size(); }
	inline int directionalLightCount() const { return m_directionalLights.size(); }
	inline int spotLightCount() const { return m_spotLights.size(); }

private:
	LightData() {}

	std::map<int, BaseLight*> m_globalLights;
	int m_globalIndex = 0;

	std::vector<PointLight> m_pointLights;
	std::vector<DirectionalLight> m_directionalLights;
	std::vector<SpotLight> m_spotLights;

public:
	LightData(LightData const&) = delete;
	void operator=(LightData const&) = delete;

};

#endif // LIGHTDATA_H