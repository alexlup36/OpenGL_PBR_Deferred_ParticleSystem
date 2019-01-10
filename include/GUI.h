#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <glm/gtc/quaternion.hpp>
#include <array>
#include <filesystem>

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
	void updateAssetList();
	void drawDirLightSettings(DirectionalLight &dirLight);
	void drawPointLightSettings(PointLight &pointLight);
	void drawSpotLightSettings(SpotLight &spotLight);
	void drawObjectSettings();
	void expandDirectory(const std::filesystem::directory_entry &dirEntry);

	inline bool isTexture(const std::string &path)
	{
		for (const std::string &textureExtension : textureExtensions)
		{
			if (path.find(textureExtension) != std::string::npos)
				return true;
		}
		return false;
	};

	inline bool isModel(const std::string &path)
	{
		for (const std::string &modelExtension : modelExtensions)
		{
			if (path.find(modelExtension) != std::string::npos)
				return true;
		}
		return false;
	};

	std::vector<const char*> m_lightSourceNames;
	std::vector<const char*> m_modelNames;
	std::vector<const char*> m_textureNames;
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
		bool m_enableDepth = false;
	};
	GbufferVisualisation m_gBufferSettings;

	std::array<const char*, 3> LightTypes = {
		"DirectionalLight",
		"PointLight",
		"SpotLight"
	};

	std::array<std::string, 2> modelExtensions = {
		".obj", ".fbx"
	};

	std::array<std::string, 4> textureExtensions = {
		".jpg", ".jpeg", ".png", ".tif"
	};

	int m_lightTypeSelection = 0;
	int m_lightSourceSelection = 0;
	int m_textureSelection = 0;
	int m_modelSelection = 0;
	bool m_lightPanelRequiresUpdate = true;

	bool m_fileSystemRequiresUpdate = true;

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