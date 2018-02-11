#include "GLFramework.h"

#include "glm\glm.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

#include "GUI.h"
#include "Input.h"
#include "Camera.h"
#include "Shader.h"
#include "Material.h"
#include "Light.h"
#include "LightData.h"
#include "MaterialData.h"

// ----------------------------------------------------------------------------

GLFramework::GLFramework(int windowWidth, int windowHeight)
	: OpenGLApp(windowWidth, windowHeight)
{
	std::cout << "GLFramework start. \n";
}

// ----------------------------------------------------------------------------

GLFramework::~GLFramework()
{
	if (m_pGUI != nullptr)
	{
		delete m_pGUI;
		m_pGUI = nullptr;
	}
}

// ----------------------------------------------------------------------------

void GLFramework::update(double dt)
{
	// Call base class
	OpenGLApp::update(dt);

	// ------------------------------------------------------------------------
	// Update here

	// Camera input
	if (Input::fpsCameraEnabled())
	{
		m_pCamera1->processInput(window(), dt);
		m_pCamera1->updateView();
	}

	// ------------------------------------------------------------------------
}

// ----------------------------------------------------------------------------

void GLFramework::draw(double dt)
{
	glCheckError();

	// Call base class
	OpenGLApp::draw(dt);

	// ------------------------------------------------------------------------
	// Do drawing here

	// Render to texture
	m_pRT->renderToTexture();

	// Do rendering
	glm::mat4 m, v, p, mvp, normalMat;

	// Get lights
	auto& pointLight1 = LightData::getInstance().pointLight1;
	auto& directionalLight1 = LightData::getInstance().directionalLight1;

	// ------------------------------------------------------------------------
	// Draw point lights
	float pointLightScale = 0.005f;
	m = glm::mat4();
	m = glm::translate(m, LightData::getInstance().pointLight1.direction);
	m = glm::scale(m, glm::vec3(pointLightScale));
	normalMat = glm::transpose(glm::inverse(m));
	p = m_pCamera1->projMatrix();
	v = m_pCamera1->viewMatrix();
	// Activate shader
	m_phongColorShader->useShader();
	// Set uniforms
	m_phongColorShader->set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_phongColorShader->set<glm::mat4>(ShaderUniform::NormalMat, normalMat);
	m_phongColorShader->set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_phongColorShader->set<glm::mat4>(ShaderUniform::ProjMat, p);
	m_phongColorShader->set<glm::vec3>(ShaderUniform::LightColor, WHITE);
	m_phongColorShader->set<glm::vec3>(ShaderUniform::LightDir, m_pGUI->m_lightDirection);
	m_phongColorShader->set<glm::vec4>(ShaderUniform::ObjectColor, WHITE);
	m_phongColorShader->set<glm::vec3>(ShaderUniform::ViewPos, m_pCamera1->viewPos());
	m_phongColorShader->setScalar<float>(ShaderUniform::Shininess, m_pGUI->m_shininess);
	m_phongColorShader->setScalar<float>(ShaderUniform::SpecularStrength, m_pGUI->m_specularStrength);
	// Draw triangles
	m_pLightModel->render();

	// ------------------------------------------------------------------------

	// Draw cube - phong

	// Set matrices
	glBindVertexArray(m_cubeVAO);
	float scaleFactor = 0.04f;
	glm::vec3 position = glm::vec3(-1.0f, 0.0f, -2.0f);
	m = glm::mat4();
	m = glm::scale(m, glm::vec3(scaleFactor));
	m = glm::translate(m, position);
	glm::mat4 rotMat = glm::toMat4(m_pGUI->m_rotation);
	m = m * rotMat;
	normalMat = glm::transpose(glm::inverse(m));
	p = m_pCamera1->projMatrix();
	v = m_pCamera1->viewMatrix();
	// Activate shader
	m_phongColorShader->useShader();
	// Set uniforms
	m_phongColorShader->set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_phongColorShader->set<glm::mat4>(ShaderUniform::NormalMat, normalMat);
	m_phongColorShader->set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_phongColorShader->set<glm::mat4>(ShaderUniform::ProjMat, p);
	m_phongColorShader->set<glm::vec3>(ShaderUniform::LightColor, m_pGUI->m_lightColor);
	m_phongColorShader->set<glm::vec3>(ShaderUniform::LightDir, m_pGUI->m_lightDirection);
	m_phongColorShader->set<glm::vec4>(ShaderUniform::ObjectColor, m_pGUI->m_objectColor);
	m_phongColorShader->set<glm::vec3>(ShaderUniform::ViewPos, m_pCamera1->viewPos());
	m_phongColorShader->setScalar<float>(ShaderUniform::Shininess, m_pGUI->m_shininess);
	m_phongColorShader->setScalar<float>(ShaderUniform::SpecularStrength, m_pGUI->m_specularStrength);
	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// ------------------------------------------------------------------------

	// Draw cube pbr

	// Set matrices
	glBindVertexArray(m_cubeVAO);
	scaleFactor = 0.08f;
	position = glm::vec3(-3.0f, 0.0f, -2.0f);
	m = glm::mat4();
	m = glm::scale(m, glm::vec3(scaleFactor));
	m = glm::translate(m, position);
	rotMat = glm::toMat4(m_pGUI->m_rotation);
	m = m * rotMat;
	normalMat = glm::transpose(glm::inverse(m));
	p = m_pCamera1->projMatrix();
	v = m_pCamera1->viewMatrix();
	// Activate shader
	m_pbr->useShader();
	// Set PBR material
	MaterialData::getInstance().matTexPBR1.bindTextures(m_pbr->program());
	// Matrices
	m_pbr->set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_pbr->set<glm::mat4>(ShaderUniform::NormalMat, normalMat);
	m_pbr->set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_pbr->set<glm::mat4>(ShaderUniform::ProjMat, p);
	// Camera
	m_pbr->set<glm::vec3>(ShaderUniform::ViewPos, m_pCamera1->viewPos());
	// Displacement mapping
	m_pbr->setScalar<float>(ShaderUniform::DisplacementMapScale, -1.0f);
	// Light
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Color, 0, pointLight1.color);
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Position, 0, pointLight1.direction);
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Attenuation, 0, pointLight1.attenuation);
	m_pbr->setDirLight<glm::vec3>(DirLightUniform::Color, 0, directionalLight1.color);
	m_pbr->setDirLight<glm::vec3>(DirLightUniform::Direction, 0, directionalLight1.direction);
	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// ---------------------------------------------------------------------
	// Draw sphere rusted iron

	// Set matrices
	scaleFactor = 0.02f;
	position = glm::vec3(-5.0f, 0.0f, -2.0f);
	m = glm::mat4();
	m = glm::scale(m, glm::vec3(scaleFactor));
	m = glm::translate(m, position);
	rotMat = glm::toMat4(m_pGUI->m_rotation);
	m = m * rotMat;
	normalMat = glm::transpose(glm::inverse(m));
	p = m_pCamera1->projMatrix();
	v = m_pCamera1->viewMatrix();
	// Activate shader
	m_pbr->useShader();
	// Set PBR material
	MaterialData::getInstance().matRustedIron.bindTextures(m_pbr->program());
	// Matrices
	m_pbr->set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_pbr->set<glm::mat4>(ShaderUniform::NormalMat, normalMat);
	m_pbr->set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_pbr->set<glm::mat4>(ShaderUniform::ProjMat, p);
	// Camera
	m_pbr->set<glm::vec3>(ShaderUniform::ViewPos, m_pCamera1->viewPos());
	// Displacement mapping
	m_pbr->setScalar<float>(ShaderUniform::DisplacementMapScale, -1.0f);
	// Light
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Color, 0, pointLight1.color);
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Position, 0, pointLight1.direction);
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Attenuation, 0, pointLight1.attenuation);
	m_pbr->setDirLight<glm::vec3>(DirLightUniform::Color, 0, directionalLight1.color);
	m_pbr->setDirLight<glm::vec3>(DirLightUniform::Direction, 0, directionalLight1.direction);
	// Draw triangles
	m_pSphereModel->render();

	// ---------------------------------------------------------------------
	// Draw sphere gold

	// Set matrices
	scaleFactor = 0.02f;
	position = glm::vec3(-10.0f, 0.0f, -2.0f);
	m = glm::mat4();
	m = glm::scale(m, glm::vec3(scaleFactor));
	m = glm::translate(m, position);
	rotMat = glm::toMat4(m_pGUI->m_rotation);
	m = m * rotMat;
	normalMat = glm::transpose(glm::inverse(m));
	p = m_pCamera1->projMatrix();
	v = m_pCamera1->viewMatrix();
	// Activate shader
	m_pbr->useShader();
	// Set PBR material
	MaterialData::getInstance().matGold.bindTextures(m_pbr->program());
	// Matrices
	m_pbr->set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_pbr->set<glm::mat4>(ShaderUniform::NormalMat, normalMat);
	m_pbr->set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_pbr->set<glm::mat4>(ShaderUniform::ProjMat, p);
	// Camera
	m_pbr->set<glm::vec3>(ShaderUniform::ViewPos, m_pCamera1->viewPos());
	// Displacement mapping
	m_pbr->setScalar<float>(ShaderUniform::DisplacementMapScale, -1.0f);
	// Light
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Color, 0, pointLight1.color);
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Position, 0, pointLight1.direction);
	m_pbr->setPointLight<glm::vec3>(PointLightUniform::Attenuation, 0, pointLight1.attenuation);
	m_pbr->setDirLight<glm::vec3>(DirLightUniform::Color, 0, directionalLight1.color);
	m_pbr->setDirLight<glm::vec3>(DirLightUniform::Direction, 0, directionalLight1.direction);
	// Draw triangles
	m_pSphereModel->render();

	// ------------------------------------------------------------------------
	// Draw torus - simple color PBR

	// Set matrices
	scaleFactor = 0.1f;
	position = glm::vec3(1.0f, 0.0f, -3.0f);
	m = glm::mat4();
	m = glm::scale(m, glm::vec3(scaleFactor));
	m = glm::translate(m, position);
	rotMat = glm::toMat4(m_pGUI->m_rotation);
	m = m * rotMat;
	normalMat = glm::transpose(glm::inverse(m));
	p = m_pCamera1->projMatrix();
	v = m_pCamera1->viewMatrix();
	// Activate shader
	m_colorPBR->useShader();
	// Set PBR material
	auto& matPBR1 = MaterialData::getInstance().matPBR1;
	m_colorPBR->setMaterial<glm::vec3>(MaterialPBRUniform::Color, matPBR1.albedo);
	m_colorPBR->setMaterialScalar<float>(MaterialPBRUniform::AmbientOcclusion, matPBR1.ao);
	m_colorPBR->setMaterialScalar<float>(MaterialPBRUniform::Roughness, matPBR1.roughness);
	m_colorPBR->setMaterialScalar<float>(MaterialPBRUniform::Metallic, matPBR1.metallic);
	// Matrices
	m_colorPBR->set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_colorPBR->set<glm::mat4>(ShaderUniform::NormalMat, normalMat);
	m_colorPBR->set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_colorPBR->set<glm::mat4>(ShaderUniform::ProjMat, p);
	// Camerea
	m_colorPBR->set<glm::vec3>(ShaderUniform::ViewPos, m_pCamera1->viewPos());
	// Light
	m_colorPBR->setPointLight<glm::vec3>(PointLightUniform::Color, 0, pointLight1.color);
	m_colorPBR->setPointLight<glm::vec3>(PointLightUniform::Position, 0, pointLight1.direction);
	m_colorPBR->setPointLight<glm::vec3>(PointLightUniform::Attenuation, 0, pointLight1.attenuation);
	m_colorPBR->setDirLight<glm::vec3>(DirLightUniform::Color, 0, directionalLight1.color);
	m_colorPBR->setDirLight<glm::vec3>(DirLightUniform::Direction, 0, directionalLight1.direction);
	
	// Draw triangles
	m_pTorusModel->render();

	// ------------------------------------------------------
	// Draw quad - parallax mapping

	position = glm::vec3(0.0f, -1.0f, -2.0f);
	scaleFactor = 0.1f;
	m = glm::mat4();
	m = glm::translate(m, position);
	m = glm::rotate(m, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, glm::vec3(scaleFactor, 0.001f, scaleFactor));
	p = m_pCamera1->projMatrix();
	v = m_pCamera1->viewMatrix();
	normalMat = glm::transpose(glm::inverse(m));
	
	// Activate shader
	m_normalMapping->useShader();
	
	// Set uniforms

	// Set textures
	/*m_brick1Diffuse->bind(m_normalMapping->program());
	m_brick1Displacement->bind(m_normalMapping->program());
	m_brick1Normal->bind(m_normalMapping->program());
	m_brick1Specular->bind(m_normalMapping->program());*/

	m_toyBoxDiffuse->bind(m_normalMapping->program());
	m_toyBoxDisplacement->bind(m_normalMapping->program());
	m_toyBoxNormal->bind(m_normalMapping->program());

	// Point light
	//m_normalMapping->setPointLight<glm::vec3&>(PointLightUniform::ColorAmbientComp, 0, pointLight1.ambientComp);
	//m_normalMapping->setPointLight<glm::vec3&>(PointLightUniform::ColorDiffuseComp, 0, pointLight1.diffuseComp);
	//m_normalMapping->setPointLight<glm::vec3&>(PointLightUniform::ColorSpecularComp, 0, pointLight1.specularComp);
	//m_normalMapping->setPointLight<glm::vec3&>(PointLightUniform::Position, 0, pointLight1.direction);
	//m_normalMapping->setPointLight<glm::vec3&>(PointLightUniform::Attenuation, 0, pointLight1.attenuation);

	// Directional light
	directionalLight1 = LightData::getInstance().directionalLight1;
	m_normalMapping->setDirLight<glm::vec3>(DirLightUniform::ColorAmbientComp, 0, directionalLight1.ambientComp);
	m_normalMapping->setDirLight<glm::vec3>(DirLightUniform::ColorDiffuseComp, 0, directionalLight1.diffuseComp);
	m_normalMapping->setDirLight<glm::vec3>(DirLightUniform::ColorSpecularComp, 0, directionalLight1.specularComp);
	m_normalMapping->setDirLight<glm::vec3>(DirLightUniform::Direction, 0, directionalLight1.direction);

	// Disp map scale
	m_normalMapping->setScalar<float>(ShaderUniform::DisplacementMapScale, m_pGUI->m_dispMapScale);
	// Normal map scale
	m_normalMapping->setScalar<float>(ShaderUniform::NormalMapScale, m_pGUI->m_normalMapScale);

	// Set material
	auto& mat1 = MaterialData::getInstance().mat1;
	m_normalMapping->setMaterial<glm::vec3>(MaterialUniform::AmbientComp, mat1.ambientComp);
	m_normalMapping->setMaterial<glm::vec3>(MaterialUniform::DiffuseComp, mat1.diffuseComp);
	m_normalMapping->setMaterial<glm::vec3>(MaterialUniform::SpecularComp, mat1.specularComp);
	m_normalMapping->setMaterialScalar<float>(MaterialUniform::Shineness, mat1.shineness);
	m_normalMapping->setScalar<float>(ShaderUniform::SpecularStrength, m_pGUI->m_specularStrength);

	// Gamma
	float gamma = 2.3f;
	m_normalMapping->setScalar<float>(ShaderUniform::Gamma, gamma);

	// Matrices
	m_normalMapping->set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_normalMapping->set<glm::mat4>(ShaderUniform::NormalMat, normalMat);
	m_normalMapping->set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_normalMapping->set<glm::mat4>(ShaderUniform::ProjMat, p);

	m_normalMapping->set<glm::vec3>(ShaderUniform::ViewPos, m_pCamera1->viewPos());
	m_normalMapping->setScalar<float>(ShaderUniform::Shininess, m_pGUI->m_shininess);

	// Debug display
	m_normalMapping->setScalar<int>(ShaderUniform::DisplayMode, static_cast<int>(m_pGUI->m_displayMode));
	
	// Draw triangles
	m_pPlaneModel->render();

	// ---------------------------------------------------------------------------

	// Render to screen
	// Activate shader
	glBindVertexArray(m_quadVAO);
	m_quadShader->useShader();
	GLuint textureUnit = 0;
	m_pRT->renderToScreen(0, 0, windowWidth(), windowHeight(), textureUnit);
	m_quadShader->setScalar<int>(ShaderUniform::RenderedTexture, textureUnit);
	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ------------------------------------------------------------------------
	// Draw GUI
	m_pGUI->draw();

	// Swap buffers
	glfwSwapBuffers(window());

	glCheckError();
}

// ----------------------------------------------------------------------------

bool GLFramework::initialize(const char* windowTitle, bool enableMultisampling, bool enableSRGB)
{
	// Base class initialize
	OpenGLApp::initialize(windowTitle, enableMultisampling, enableSRGB);

	std::cout << "GLFramework initialize. \n";

	// ------------------------------------------------------------------------
	// Initialization

	// Initialize GUI
	m_pGUI = new GUI();
	m_pGUI->setup(window(), windowWidth(), windowHeight());

	// Initialize input
	Input::getInstance().initialize(window());

	// Initalize camera
	m_pCamera1 = std::make_unique<Camera>(window(), windowWidth(), windowHeight());

	// Initialize RT
	m_pRT = std::make_unique<RenderTarget>(GL_RGBA16F,
		GL_RGBA,
		GL_FLOAT,
		windowWidth(),
		windowHeight(),
		true,
		GL_DEPTH_COMPONENT24);
	if (m_pRT->initialize() == false)
	{
		std::cout << "Failed to create RT. \n";
		return false;
	}

	// ------------------------------------------------------------------------
	// Setup scene
	setupScene();

	// Success
	return true;
}

// ----------------------------------------------------------------------------

void GLFramework::setupScene()
{
	std::cout << "Scene setup. \n";

	// Initialize lights
	LightData::getInstance().initialize();
	MaterialData::getInstance().initialize();

	// Load shaders
	m_basicShader = std::make_unique<Shader>(".\\Shaders\\basic.vert", ".\\Shaders\\basic.frag");
	m_quadShader = std::make_unique<Shader>(".\\Shaders\\quad.vert", ".\\Shaders\\quad.frag");
	m_phongColorShader = std::make_unique<Shader>(".\\Shaders\\phongColor.vert", ".\\Shaders\\phongColor.frag");
	m_phongTextureShader = std::make_unique<Shader>(".\\Shaders\\phongTexture.vert", ".\\Shaders\\phongTexture.frag");
	m_normalMapping = std::make_unique<Shader>(".\\Shaders\\normalMapping.vert", ".\\Shaders\\normalMapping.frag");
	m_parallaxMapping = std::make_unique<Shader>(".\\Shaders\\parallaxMapping.vert", ".\\Shaders\\parallaxMapping.frag");
	m_colorPBR = std::make_unique<Shader>(".\\Shaders\\simplePBR.vert", ".\\Shaders\\simplePBR.frag");
	m_pbr = std::make_unique<Shader>(".\\Shaders\\pbr.vert", ".\\Shaders\\pbr.frag");

	// Load textures
	m_brick1Diffuse = TextureMan::Instance().getTexture("..//Assets//Textures//brick1//brick_diffuse.jpg", TextureType::Diffuse1);
	m_brick1Displacement = TextureMan::Instance().getTexture("..//Assets//Textures//brick1/brick_displacement.jpg", TextureType::Displacement);
	m_brick1Normal = TextureMan::Instance().getTexture("..//Assets//Textures//brick1//brick_normal.jpg", TextureType::Normal1);
	m_brick1Specular = TextureMan::Instance().getTexture("..//Assets//Textures//brick1//brick_specular.jpg", TextureType::Specular);

	m_toyBoxDiffuse = TextureMan::Instance().getTexture("..//Assets//Textures//toybox//toy_box_diffuse.png", TextureType::Diffuse1);
	m_toyBoxDisplacement = TextureMan::Instance().getTexture("..//Assets//Textures//toybox//toy_box_disp.png", TextureType::Displacement);
	m_toyBoxNormal = TextureMan::Instance().getTexture("..//Assets//Textures//toybox//toy_box_normal.png", TextureType::Normal1);

	m_brick1RoughnessPBR = TextureMan::Instance().getTexture("..//Assets//Textures//brick1//brick_specular.jpg", TextureType::Roughness);
	m_brick1MetallnessPBR = TextureMan::Instance().getTexture("..//Assets//Textures//brick1/brick_specular.jpg", TextureType::Metalness);
	m_brick1AmbientOcclusionPBR = TextureMan::Instance().getTexture("..//Assets//Textures//brick1//brick_displacement.jpg", TextureType::AmbientOcclusion);

	m_rustedIronAlbedo = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//rusted_iron//albedo.png", TextureType::Diffuse1);
	m_rustedIronRoughness = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//rusted_iron//metallic.png", TextureType::Metalness);
	m_rustedIronMetallic = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//rusted_iron//roughness.png", TextureType::Roughness);
	m_rustedIronAmbientOcclusion = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//rusted_iron//ao.png", TextureType::AmbientOcclusion);
	m_rustedIronNormal = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//rusted_iron//normal.png", TextureType::Normal1);

	m_goldAlbedo = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//gold//albedo.png", TextureType::Diffuse1);
	m_goldRoughness = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//gold//metallic.png", TextureType::Metalness);
	m_goldMetallic = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//gold//roughness.png", TextureType::Roughness);
	m_goldAmbientOcclusion = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//gold//ao.png", TextureType::AmbientOcclusion);
	m_goldNormal = TextureMan::Instance().getTexture("..//Assets//Textures//pbr//gold//normal.png", TextureType::Normal1);

	// Setup PBR material

	// Test brick
	MaterialData::getInstance().matTexPBR1.albedo = m_brick1Diffuse;
	MaterialData::getInstance().matTexPBR1.normal = m_brick1Normal;
	MaterialData::getInstance().matTexPBR1.displacement = m_brick1Displacement;
	MaterialData::getInstance().matTexPBR1.metallic = m_brick1MetallnessPBR;
	MaterialData::getInstance().matTexPBR1.roughness = m_brick1RoughnessPBR;
	MaterialData::getInstance().matTexPBR1.ao = m_brick1AmbientOcclusionPBR;

	// Rusted iron
	MaterialData::getInstance().matRustedIron.albedo = m_rustedIronAlbedo;
	MaterialData::getInstance().matRustedIron.metallic = m_rustedIronMetallic;
	MaterialData::getInstance().matRustedIron.roughness = m_rustedIronRoughness;
	MaterialData::getInstance().matRustedIron.ao = m_rustedIronAmbientOcclusion;
	MaterialData::getInstance().matRustedIron.normal = m_rustedIronNormal;

	// Gold
	MaterialData::getInstance().matGold.albedo = m_goldAlbedo;
	MaterialData::getInstance().matGold.metallic = m_goldMetallic;
	MaterialData::getInstance().matGold.roughness = m_goldRoughness;
	MaterialData::getInstance().matGold.ao = m_goldAmbientOcclusion;
	MaterialData::getInstance().matGold.normal = m_goldNormal;

	// Load meshes
	m_pTorusModel = std::make_unique<Model<VertexPN>>("..//Assets//torus.obj");
	//m_pMonkeyModel = std::make_unique<Model<VertexPN>>("..//Assets//mymodel.obj");
	m_pPlaneModel = std::make_unique<Model<VertexPTNT>>("..//Assets//plane2.obj");
	m_pLightModel = std::make_unique<Model<VertexPN>>("..//Assets//sphere.obj");
	m_pSphereModel = std::make_unique<Model<VertexPTNT>>("..//Assets//planet.obj");

	// Generate VAO
	m_cubeVAO = Mesh<int>::vaoCubeSetup();
	m_quadVAO = Mesh<int>::vaoQuadSetup();

	// Generate and bind vertex array object (Required for OpenGL context > 3.1)
	GLuint vertexArrayObject = 0;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
}

// ----------------------------------------------------------------------------
