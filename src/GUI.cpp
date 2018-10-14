#include <assert.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "Input.h"
#include <GLFW/glfw3.h>
#include "GUI.h"

#include "Material.h"
#include "LightData.h"
#include "MaterialData.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GUI::GUI(GLFWwindow* window)
	: m_window(window)
{
	assert(window && "Invalid GLFW window.");

	// Setup imgui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup imgui for GLFW and OpenGL3+
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsDark();
}

GUI::~GUI()
{
}

void GUI::draw() 
{
	// Start imgui new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Setup
	ImGui::Begin("MainWindow");

	if (ImGui::CollapsingHeader("Properties"))
	{
		// Nornal map
		ImGui::SliderFloat("Normal map scale", &m_normalMapScale, 1.0f, 5.0f);

		ImGui::Separator();

		// Texture
		ImGui::SliderFloat2("Texture offset", &m_textureOffset.x, 0.0f, 1.0f);
		ImGui::SliderFloat2("Texture tile", &m_textureTile.x, 0.0f, 5.0f);

		ImGui::Separator();

		// Frame
		ImGui::ColorEdit3("Clear color", (float*)&m_clearColor);
		ImGui::Checkbox("VSync", &m_enableVsync);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	if (ImGui::CollapsingHeader("Gbuffer Visualisation"))
	{
		drawGbufferSettings();
	}

	if (ImGui::CollapsingHeader("Light properties"))
	{
		drawLightPanel();
	}
	
    ImGui::End();

	// Rendering
	ImGui::Render();
	glfwGetFramebufferSize(m_window, &m_framebufferWidth, &m_framebufferHeight);
	glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::drawGbufferSettings()
{
	ImGui::Checkbox("Enable position buffer", &m_gBufferSettings.m_enablePosition);
	ImGui::Checkbox("Enable albedo buffer", &m_gBufferSettings.m_enableAlbedo);
	ImGui::Checkbox("Enable PBR buffer", &m_gBufferSettings.m_enablePBR);
	ImGui::Checkbox("Enable normal buffer", &m_gBufferSettings.m_enableNormal);
}

void GUI::drawLightPanel()
{
	// Display list of light source type
	ImGui::ListBox("Light types", &m_lightTypeSelection, LightTypes.data(), LightTypes.size());
	// Add light source button
	bool addLightSourcePressed = ImGui::Button("Add light source");

	ImGui::ListBox("Light sources", &m_lightSourceSelection, m_lightSourceNames.data(), m_lightSourceNames.size());
	// Display 
	auto baseLight = LightData::getInstance().getLight(m_lightSourceSelection);
	auto lightSourceType = baseLight->lightSourceType;
	switch (lightSourceType)
	{
		case LightSourceType::Directional:
			drawDirLightSettings(*(static_cast<DirectionalLight*>(baseLight)));
			break;
		case LightSourceType::Point:
			drawPointLightSettings(*(static_cast<PointLight*>(baseLight)));
			break;
		case LightSourceType::Spot:
			drawSpotLightSettings(*(static_cast<SpotLight*>(baseLight)));
			break;
		default:
			std::cout << "Invalid light type selection.\n";
			break;
	}

	if (addLightSourcePressed)
	{
		m_lightPanelRequiresUpdate = true;

		// Add new light source
		switch (m_lightTypeSelection)
		{
			case 0:
			{
				DirectionalLight newDirLight;
				LightData::getInstance().addDirectionalLight(newDirLight);
				break;
			}
			case 1:
			{
				PointLight newPointLight;
				LightData::getInstance().addPointLight(newPointLight);
				break;
			}
			case 2:
			{
				SpotLight newSpotLight;
				LightData::getInstance().addSpotLight(newSpotLight);
				break;
			}
			default:
				std::cout << "Invalid light selection.\n";
				break;
		}
	}

	if (m_lightPanelRequiresUpdate) updateLightSourcesList();
}

void GUI::updateLightSourcesList()
{
	m_lightSourceNames.clear();
	m_lightSources = LightData::getInstance().getIds();
	for (std::string &lightSource : m_lightSources)
		m_lightSourceNames.push_back(lightSource.c_str());

	m_lightPanelRequiresUpdate = false;
}

void GUI::drawDirLightSettings(DirectionalLight &dirLight)
{
	ImGui::Checkbox("Enable", &dirLight.enabled);
	ImGui::Checkbox("PBR color settings", &dirLight.pbrLight);
	if (dirLight.pbrLight)
	{
		ImGui::ColorEdit3("Color", (float*)&dirLight.color);
	}
	else
	{
		ImGui::ColorEdit3("AmbientComponent", (float*)&dirLight.ambientComp);
		ImGui::ColorEdit3("DiffuseComponent", (float*)&dirLight.diffuseComp);
		ImGui::ColorEdit3("SpecularComponent", (float*)&dirLight.specularComp);
	}
	ImGui::SliderFloat3("Direction", &dirLight.direction.x, -1.0f, 1.0f);
}

void GUI::drawPointLightSettings(PointLight &pointLight)
{
	ImGui::Checkbox("Enable", &pointLight.enabled);
	ImGui::Checkbox("PBR color settings", &pointLight.pbrLight);
	if (pointLight.pbrLight)
	{
		ImGui::ColorEdit3("Color", (float*)&pointLight.color);
	}
	else
	{
		ImGui::ColorEdit3("AmbientComponent", (float*)&pointLight.ambientComp);
		ImGui::ColorEdit3("DiffuseComponent", (float*)&pointLight.diffuseComp);
		ImGui::ColorEdit3("SpecularComponent", (float*)&pointLight.specularComp);
	}
	ImGui::SliderFloat3("Position", &pointLight.position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Attenuation", &pointLight.attenuation.x, 0.0f, 10.0f);
}

void GUI::drawSpotLightSettings(SpotLight &spotLight)
{
	ImGui::Checkbox("Enable", &spotLight.enabled);
	ImGui::Checkbox("PBR color settings", &spotLight.pbrLight);	
	if (spotLight.pbrLight)
	{
		ImGui::ColorEdit3("Color", (float*)&spotLight.color);
	}
	else
	{
		ImGui::ColorEdit3("AmbientComponent", (float*)&spotLight.ambientComp);
		ImGui::ColorEdit3("DiffuseComponent", (float*)&spotLight.diffuseComp);
		ImGui::ColorEdit3("SpecularComponent", (float*)&spotLight.specularComp);
	}
	ImGui::SliderFloat3("Position", &spotLight.position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Direction", &spotLight.direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Coscutoff", &spotLight.coscutoff, -10.0f, 10.0f);
	ImGui::SliderFloat("Cutoff", &spotLight.cutoff, -10.0f, 10.0f);
	ImGui::SliderFloat("Exponent", &spotLight.exponent, -10.0f, 10.0f);
}