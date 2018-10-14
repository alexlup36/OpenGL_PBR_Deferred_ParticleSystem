#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <glm/gtc/quaternion.hpp>
#include <array>

#include "Light.h"

class GUI 
{
public:
	GUI(GLFWwindow* window);
	~GUI();
	void draw();

private:
	void drawGbufferSettings();
	void drawLightPanel();
	void updateLightSourcesList();
	void drawDirLightSettings(DirectionalLight &dirLight);
	void drawPointLightSettings(PointLight &pointLight);
	void drawSpotLightSettings(SpotLight &spotLight);

	std::vector<const char*> m_lightSourceNames;
	std::vector<std::string> m_lightSources;

public:

	enum class DisplayMode
	{
		DIFFUSE = 0,
		NORMAL = 1,
		NORMAL_TEX = 2,
		DIRLIGHT_SHADING = 3,
		POINTLIGHT_SHADING = 4,
		FINAL = 5,

		COUNT,
	};

	enum class ToneMapper 
	{
		NORMAL = 0,
		GAMMA_CORRECTED,
		REINHARD,
		EXPOSURE_TONE_MAP,
		JIM_TONE_MAP,
		UNCHARTED,

		COUNT,
	};

	struct GbufferVisualisation
	{
		bool m_enableAlbedo = false;
		bool m_enablePBR = false;
		bool m_enableNormal = false;
		bool m_enablePosition = false;
	};
	GbufferVisualisation m_gBufferSettings;

	std::array<const char*, 3> LightTypes = {
		"DirectionalLight",
		"PointLight",
		"SpotLight"
	};
	int m_lightTypeSelection = 0;
	int m_lightSourceSelection = 0;
	bool m_lightPanelRequiresUpdate = true;

	glm::quat m_rotation = glm::quat();
	glm::vec2 m_textureOffset = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_textureTile = glm::vec2(1.0f, 1.0f);
	float m_shininess = 32.0f;
	float m_specularStrength = 0.1f;
	float m_dispMapScale = 0.0f;
	float m_normalMapScale = 1.0f;
	float m_gamma = 2.2f;
	float m_gammaHDR = 1.0f;
	float m_exposureBias = 2.0f;
	float m_exposure = 1.0f;
	glm::vec3 m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 m_objectColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 m_clearColor = glm::vec3(0.2f, 0.2f, 0.2f);
	GLFWwindow *m_window = nullptr;
	bool m_enableVsync = true;

	int m_framebufferWidth, m_framebufferHeight;

	DisplayMode m_displayMode = DisplayMode::FINAL;
	ToneMapper m_toneMapper = ToneMapper::NORMAL;
};

#endif // GUI_H