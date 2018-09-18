#ifndef GUI_H
#define GUI_H

#include <stdio.h>
//#include <AntTweakBar.h>
#include <glm/gtc/quaternion.hpp>

class GUI 
{
public:
	GUI(GLFWwindow* window);
	~GUI();
	bool setup(GLFWwindow* window, int w, int h);
	void draw();
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

	//TwBar* bar;
	double test;
	/*float m_rotation[4] = { 0.5f, 0.5f, 0.5f };
	float m_lightDirection[3] = { 0.5f, 0.5f, 0.5f };
	float m_shininess = 32.0f;
	float m_lightColor[3] = { 1.0f, 1.0f, 1.0f };
	float m_objectColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };*/

	//glm::vec4 m_rotation = glm::vec4(0.5f, 0.5f, 0.5f, 0.0f);
	glm::quat m_rotation = glm::quat();
	glm::vec3 m_lightDirection = glm::vec3(0.5f, 0.5f, 0.5f);
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

	//PointLight* m_pCurrentPointLight = LightData::getInstance().pointLight1;
};

#endif // GUI_H