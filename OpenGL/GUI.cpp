#include <assert.h>
#include <iostream>
#include <string>

#include "Input.h"
#include <GLFW/glfw3.h>
#include "GUI.h"

#include "Light.h"
#include "Material.h"
#include "LightData.h"
#include "MaterialData.h"

GUI::GUI()
{
}

GUI::~GUI()
{
	TwTerminate();
}

void TW_CALL setFPSCamera(const void *value, void * /*clientData*/)
{
	Input::setFPSCameraEnabled(*static_cast<const bool*>(value));
}


// Callback function called by AntTweakBar to get ambient occlusion state
void TW_CALL getFPSCamera(void *value, void * /*clientData*/)
{
	*static_cast<bool *>(value) = Input::fpsCameraEnabled();
}

bool GUI::setup(GLFWwindow* window, int w, int h) 
{
	assert(w);
	assert(h);
	assert(window);

	TwInit(TW_OPENGL_CORE, NULL);

	bar = TwNewBar("Phong Lighting");

	TwAddVarRW(bar, "TextureOffset X", TW_TYPE_FLOAT, &m_textureOffset.x, "min=0 max=10 step=0.01 ");
	TwAddVarRW(bar, "TextureOffset Y", TW_TYPE_FLOAT, &m_textureOffset.y, "min=0 max=10 step=0.01 ");
	TwAddVarRW(bar, "TextureTile X", TW_TYPE_FLOAT, &m_textureTile.x, "min=0 max=10 step=0.01 ");
	TwAddVarRW(bar, "TextureTile Y", TW_TYPE_FLOAT, &m_textureTile.y, "min=0 max=10 step=0.01 ");

	TwAddVarRW(bar, "ObjectShininess", TW_TYPE_FLOAT, &m_shininess, "min=1 max=256 step=0.1 help='Power of 2'");
	TwAddVarRW(bar, "ObjectSpecularStrength", TW_TYPE_FLOAT, &m_specularStrength, "min=0 max=1 step=0.01 ");
	TwAddVarRW(bar, "ObjectDispMapScale", TW_TYPE_FLOAT, &m_dispMapScale, "min=0 max=5 step=0.001 ");
	TwAddVarRW(bar, "NormalMapScale", TW_TYPE_FLOAT, &m_normalMapScale, "min=0 max=5 step=0.1 ");
	TwAddVarRW(bar, "Gamma", TW_TYPE_FLOAT, &m_gamma, "min=0 max=10 step=0.01 ");
	TwAddVarRW(bar, "GammaHDR", TW_TYPE_FLOAT, &m_gammaHDR, "min=0 max=10 step=0.01 ");
	TwAddVarRW(bar, "Exposure", TW_TYPE_FLOAT, &m_exposure, "min=0 max=10 step=0.01 ");
	TwAddVarRW(bar, "ExposureBias", TW_TYPE_FLOAT, &m_exposureBias, "min=0 max=10 step=0.01 ");

	//TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, m_rotation,
	//	" label='Object rotation' opened=true help='Change the object orientation.' ");

	//TwAddVarRW(bar, "LightDirection", TW_TYPE_DIR3F, m_lightDirection,
	//	" label='Light direction' opened=true help='Change the light direction.' ");

	//TwAddVarRW(bar, "ObjectColor", TW_TYPE_COLOR4F, m_objectColor,
	//	" label='Cube color' alpha help='Color and transparency of the cube.' ");

	//TwAddVarRW(bar, "LightColor", TW_TYPE_COLOR3F, m_lightColor, " label='Light color' ");

	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, static_cast<void*>(&m_rotation[0]),
		" label='Object rotation' opened=true help='Change the object orientation.' ");

	TwAddVarRW(bar, "LightDirection", TW_TYPE_DIR3F, static_cast<void*>(&LightData::getInstance().directionalLight1.direction[0]),
		" label='Light direction' opened=true help='Change the light direction.' ");

	TwAddVarRW(bar, "ObjectColor", TW_TYPE_COLOR4F, static_cast<void*>(&m_objectColor[0]),
		" label='Cube color' alpha help='Color and transparency of the cube.' ");

	TwAddVarRW(bar, "LightColor", TW_TYPE_COLOR3F, static_cast<void*>(&m_lightColor[0]), " label='Light color' ");

	TwAddVarCB(bar, "FPS Camera", TW_TYPE_BOOLCPP, setFPSCamera, getFPSCamera, NULL, "label='FPS Camera'");

	// Display mode
	TwEnumVal display[static_cast<int>(DisplayMode::COUNT)] = {
		{ static_cast<int>(DisplayMode::DIFFUSE), "Albedo" },
		{ static_cast<int>(DisplayMode::NORMAL), "VertexNormal" },
		{ static_cast<int>(DisplayMode::NORMAL_TEX), "TextureNormal" },
		{ static_cast<int>(DisplayMode::DIRLIGHT_SHADING), "Dir light shading" },
		{ static_cast<int>(DisplayMode::POINTLIGHT_SHADING), "Point light shading" },
		{ static_cast<int>(DisplayMode::FINAL), "Final color" } };

	TwType displayType = TwDefineEnum("DisplayMode", display, static_cast<int>(DisplayMode::COUNT));
	TwAddVarRW(bar, "DisplayMode", displayType, &m_displayMode, " label='Display Mode' ");

	// Tone mapper
	TwEnumVal toneMapper[static_cast<int>(ToneMapper::COUNT)] = {
		{ static_cast<int>(ToneMapper::NORMAL), "Normal" },
		{ static_cast<int>(ToneMapper::GAMMA_CORRECTED), "Gamma corrected" },
		{ static_cast<int>(ToneMapper::REINHARD), "Reinhard" },
		{ static_cast<int>(ToneMapper::EXPOSURE_TONE_MAP), "Exposure tone map" },
		{ static_cast<int>(ToneMapper::JIM_TONE_MAP), "Jim tone map" },
		{ static_cast<int>(ToneMapper::UNCHARTED), "Uncharted tone map" } };

	TwType toneMapperType = TwDefineEnum("ToneMapper", toneMapper, static_cast<int>(ToneMapper::COUNT));
	TwAddVarRW(bar, "ToneMap", toneMapperType, &m_toneMapper, " label='Tone Mapper' ");

	enum lightType
	{
		DIR_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
	};
	lightType dLight0 = lightType::DIR_LIGHT;
	lightType pLight0 = lightType::POINT_LIGHT;
	const int dirLightCount = 2;
	TwEnumVal dirLight[dirLightCount] = { {lightType::POINT_LIGHT, "dirLight0"}, {lightType::DIR_LIGHT, "dirLight1"} };
	TwType dirLightType = TwDefineEnum("DirLight", dirLight, dirLightCount);
	int x = TwAddVarRW(bar, "DirLights", dirLightType, &dLight0, "");
	x = TwAddVarRW(bar, "DirLights", dirLightType, &pLight0, "");

	//{
	//	// ShapeEV associates Shape enum values with labels that will be displayed instead of enum values
	//	TwEnumVal shapeEV[NUM_SHAPES] = { { SHAPE_TEAPOT, "Teapot" },{ SHAPE_TORUS, "Torus" },{ SHAPE_CONE, "Cone" } };
	//	// Create a type for the enum shapeEV
	//	TwType shapeType = TwDefineEnum("ShapeType", shapeEV, NUM_SHAPES);
	//	// add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
	//	TwAddVarRW(bar, "Shape", shapeType, &g_CurrentShape, " keyIncr='<' keyDecr='>' help='Change object shape.' ");
	//}

	// -----------------------------------------------------------------
	// Light setup
	TwStructMember lightMembers[] =
	{
		{ "Position.x",    TW_TYPE_FLOAT, offsetof(PointLight, direction.x),    " step=0.001 help='Point light posiion X' " },
		{ "Position.y",    TW_TYPE_FLOAT, offsetof(PointLight, direction.y),    " step=0.001 help='Point light posiion Y.' " },
		{ "Position.z",    TW_TYPE_FLOAT, offsetof(PointLight, direction.z),    " step=0.001 help='Point light posiion Z' " },
		{ "Color",     TW_TYPE_COLOR3F, offsetof(PointLight, color),     " noalpha help='Light color.' " },
		{ "Attenuation.x",    TW_TYPE_FLOAT,   offsetof(PointLight, attenuation.x),    " min=0 step=0.01 help='Point light attenuation quadratic.' " },
		{ "Attenuation.y",    TW_TYPE_FLOAT,   offsetof(PointLight, attenuation.y),    " min=0 step=0.01 help='Point light attenuation linear.' " },
		{ "Attenuation.z",    TW_TYPE_FLOAT,   offsetof(PointLight, attenuation.z),    " min=0 step=0.01 help='Point light attenuation constant.' " },
	};
	TwType pointLightType = TwDefineStruct("PointLight", lightMembers, 7, sizeof(PointLight), NULL, NULL);

	for (int lightIndex = 0; lightIndex < 1; ++lightIndex) 
	{                 
		auto& pointLight = LightData::getInstance().pointLight1;

		std::string sLightIndex = std::to_string(lightIndex);
		std::string pointLightName = "pointLight[" + sLightIndex + "]";
		TwAddVarRW(bar, pointLightName.c_str(), pointLightType, &pointLight, " group='Edit point lights' ");
	}
	// -----------------------------------------------------------------
	// Material setup

	// -----------------------------------------------------------------
	// PBR material setup
	TwStructMember pbrMatMembers[] =
	{
		{ "Albedo",    TW_TYPE_COLOR3F, offsetof(MaterialColorPBR, albedo),    " help='Main material color.' " },
		{ "Metallic",     TW_TYPE_FLOAT, offsetof(MaterialColorPBR, metallic),     " min=0 max=1 step=0.01 help='Metallic value.' " },
		{ "Roughness",    TW_TYPE_FLOAT,   offsetof(MaterialColorPBR, roughness),    " min=0 max=1 step=0.01 help='Roughness value.' " },
		{ "AmbientOcclusion",    TW_TYPE_FLOAT,   offsetof(MaterialColorPBR, ao),    " min=0 max=1 step=0.01 help='Ambient occlusion value.' " }
	};
	TwType pbrMatType = TwDefineStruct("PBRMaterial", pbrMatMembers, 4, sizeof(MaterialColorPBR), NULL, NULL);

	auto& pbrMat = MaterialData::getInstance().matPBR1;
	int pbrMatIndex = 0;
	std::string sPbrMatIndex = std::to_string(pbrMatIndex);
	std::string pbrMatName = "pbrMat[" + sPbrMatIndex + "]";
	TwAddVarRW(bar, pbrMatName.c_str(), pbrMatType, &pbrMat, " group='Edit PBR materials' ");

	// -----------------------------------------------------------------

	TwWindowSize(w, h);

	return true;
}

void GUI::draw() 
{
	TwDraw();
}