#include "GLFramework.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "GUI.h"
#include "Input.h"
#include "Material.h"
#include "Light.h"
#include "LightData.h"
#include "MaterialData.h"
#include "Window.h"
#include "CameraMan.h"

#include "ParticleSystem/ParticleSystem.h"

// ----------------------------------------------------------------------------

GLFramework::GLFramework(int windowWidth, int windowHeight, const CameraMan& cameraMan)
	: OpenGLApp(windowWidth, windowHeight),
	m_cameraMan(cameraMan)
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
	glCheckError();

	// Call base class
	OpenGLApp::update(dt);
	glfwSwapInterval(static_cast<int>(m_pGUI->m_enableVsync));

	glCheckError();

	// ------------------------------------------------------------------------
	// Update here

	//ParticleSystem::instance().update((float)dt);

	// Camera input
	if (Input::fpsCameraEnabled())
	{
		m_cameraMan.getActiveCamera()->processInput(window(), dt);

		glCheckError();

		m_cameraMan.getActiveCamera()->updateView();

		glCheckError();
	}

	// ------------------------------------------------------------------------
}

// ----------------------------------------------------------------------------

void GLFramework::draw(double dt)
{
	glCheckError();

	auto clearColor = m_pGUI->m_clearColor;
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);

	// Call base class
	OpenGLApp::draw(dt);

	glCheckError();

	// ------------------------------------------------------------------------
	// Do drawing here

	// Render to the depth map setup
	//m_shadowFramebuffer.renderToTexture(Framebuffer::RenderTargetType::DEPTH_TARGET);

	//glCheckError();

	// Render scene to depth map
	//drawSceneToDepth();

	//glCheckError();

	// Render to texture setup
	//m_displayFramebuffer.renderToTexture(Framebuffer::RenderTargetType::COLOR_TARGET);

	//glCheckError();

	// Draw scene
	//drawScene(dt);

	//glCheckError();

	// ------------------------------------------------------------------------
	// Scene rendering

	drawToGBuffer(dt);
	drawDeferredLighting(dt);
	drawForwardLighting(dt);
	drawToBackBuffer();

	// Particle rendering 
	//ParticleSystem::instance().draw();

	// Debug rendering
	drawGbufferToScreen();

	// Draw GUI
	m_pGUI->draw();

	// ------------------------------------------------------------------------
	// End frame

	// Swap buffers
	glfwSwapBuffers(window());

	glCheckError();
}

void GLFramework::drawGbufferToScreen()
{
#ifndef NDEBUG
	glCheckError();
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, userEventID, -1, "GBufferToScreenPass");
#endif // NDEBUG

	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	// Draw gbuffer to screen
	m_quadShader.useShader();
	glBindVertexArray(m_quadVAO);
	GLuint textureUnit;
	int leftOffset = 0;
	int horizontalSpacing = 0;
	int gbufferVisualisationWidth = 200;
	int gbufferVisualisationHeight = 130;

	if (m_pGUI->m_gBufferSettings.m_enablePosition)
	{
		// Position
		textureUnit = 0;
		int left = leftOffset;
		m_gbufferFramebuffer.renderColorTargetToScreen(left, 0, gbufferVisualisationWidth, gbufferVisualisationHeight, textureUnit);
		m_quadShader.setScalar<unsigned int>(ShaderUniform::RenderedTexture, textureUnit);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		leftOffset += (gbufferVisualisationWidth + horizontalSpacing);
	}

	if (m_pGUI->m_gBufferSettings.m_enableNormal)
	{
		// Normal
		textureUnit = 1;
		int left = leftOffset;
		m_gbufferFramebuffer.renderColorTargetToScreen(left, 0, gbufferVisualisationWidth, gbufferVisualisationHeight, textureUnit);
		m_quadShader.setScalar<unsigned int>(ShaderUniform::RenderedTexture, textureUnit);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		leftOffset += (gbufferVisualisationWidth + horizontalSpacing);
	}

	if (m_pGUI->m_gBufferSettings.m_enableAlbedo)
	{
		// Albedo
		textureUnit = 2;
		int left = leftOffset;
		m_gbufferFramebuffer.renderColorTargetToScreen(left, 0, gbufferVisualisationWidth, gbufferVisualisationHeight, textureUnit);
		m_quadShader.setScalar<unsigned int>(ShaderUniform::RenderedTexture, textureUnit);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		leftOffset += (gbufferVisualisationWidth + horizontalSpacing);
	}

	if (m_pGUI->m_gBufferSettings.m_enablePBR)
	{
		// PBR
		textureUnit = 3;
		int left = leftOffset;
		m_gbufferFramebuffer.renderColorTargetToScreen(left, 0, gbufferVisualisationWidth, gbufferVisualisationHeight, textureUnit);
		m_quadShader.setScalar<unsigned int>(ShaderUniform::RenderedTexture, textureUnit);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		leftOffset += (gbufferVisualisationWidth + horizontalSpacing);
	}

	m_quadDepthShader.useShader();
	glBindVertexArray(m_quadVAO);
	if (m_pGUI->m_gBufferSettings.m_enableDepth)
	{
		// Depth
		textureUnit = 4;
		int left = leftOffset;
		m_gbufferFramebuffer.renderDepthTargetToScreen(left, 0, gbufferVisualisationWidth, gbufferVisualisationHeight, textureUnit);
		m_quadDepthShader.setScalar<unsigned int>(ShaderUniform::RenderedTexture, textureUnit);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		leftOffset += (gbufferVisualisationWidth + horizontalSpacing);
	}

#ifndef NDEBUG
	glPopDebugGroup();
	glCheckError();
#endif // NDEBUG
}

// ----------------------------------------------------------------------------

void GLFramework::drawToBackBuffer()
{
#ifndef NDEBUG
	glCheckError();
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, userEventID, -1, "BackBufferPass");
#endif // NDEBUG

	// Render color to screen
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);
	// Activate shader
	glBindVertexArray(m_quadVAO);
	m_finalShader.useShader();
	GLuint textureUnit = 0;
	m_displayFramebuffer.renderColorTargetToScreen(0, 0, windowWidth(), windowHeight(), textureUnit);
	// Set tone mapper
	m_finalShader.setScalar<unsigned int>(ShaderUniform::ToneMapper, static_cast<unsigned int>(m_pGUI->m_toneMapper));
	m_finalShader.setScalar<float>(ShaderUniform::GammaHDR, m_pGUI->m_gammaHDR);
	m_finalShader.setScalar<float>(ShaderUniform::Exposure, m_pGUI->m_exposure);
	m_finalShader.setScalar<float>(ShaderUniform::ExposureBias, m_pGUI->m_exposureBias);
	m_finalShader.setScalar<unsigned int>(ShaderUniform::RenderedTexture, textureUnit);
	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 6);

#ifndef NDEBUG
	glPopDebugGroup();
	glCheckError();
#endif // NDEBUG
}

// ----------------------------------------------------------------------------

bool GLFramework::initialize(const char* windowTitle, bool enableMultisampling, bool enableSRGB)
{
	// ------------------------------------------------------------------------

	// Base class initialize
	OpenGLApp::initialize(windowTitle, enableMultisampling, enableSRGB);

	glCheckError();

	std::cout << "GLFramework initialize. \n";

	// ------------------------------------------------------------------------
	// Initialization

	// Initialize GUI
	m_pGUI = new GUI(window());

	// Initialize input
	Input::getInstance().initialize(window());
	Window windowData{};
	windowData.window = window();
	windowData.windowWidth = windowWidth();
	windowData.windowHeight = windowHeight();

	// Initalize camera
	CameraMan::Instance().createCamera(windowData, "FPSCamera1", 60.0f, 0.001f, 1000.0f);

	// ------------------------------------------------------------------------

	// Particle system setup
	auto &ps = ParticleSystem::instance();
	ps.setCamera(m_cameraMan.getActiveCamera());
	auto pointGenerator0 = ps.addGenerator(Generator::Type::Point, 10000);

	// ------------------------------------------------------------------------

	glCheckError();

	// Initialize RT
	m_displayWidth = windowWidth();
	m_displayHeight = windowHeight();
	m_depthMapWidth = 1024;
	m_depthMapHeight = 1024;

	// Create display buffer
	m_displayFramebuffer
		.initialize(m_displayWidth, m_displayHeight)
		.addColorTarget("DisplayColor", GL_RGBA16F, GL_RGBA, GL_FLOAT);
	if (m_displayFramebuffer.create() == false)
	{
		std::cout << "Failed to initialize display framebuffer.\n";
		return false;
	}

	// Create depth framebuffer
	m_shadowFramebuffer.initialize(m_depthMapWidth, m_depthMapHeight)
		.addDepthTarget(GL_DEPTH_COMPONENT24);
	if (m_shadowFramebuffer.create() == false)
	{
		std::cout << "Failed to initialize shadow framebuffer.\n";
		return false;
	}

	// Create g buffer
	m_gbufferFramebuffer.initialize(m_displayWidth, m_displayHeight)
		.addColorTarget("Position", GL_RGB16F, GL_RGB, GL_FLOAT)
		.addColorTarget("Normal", GL_RGB16F, GL_RGB, GL_FLOAT)
		.addColorTarget("Albedo", GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE)
		.addColorTarget("PBR", GL_RGB16F, GL_RGB, GL_FLOAT)
		.addDepthTarget(GL_DEPTH_COMPONENT24);
	if (m_gbufferFramebuffer.create() == false)
	{
		std::cout << "Failed to initialize the g buffer.\n";
		return false;
	}

	glCheckError();

	// Create the Texture2D object using the depth texture handler
	m_depthMap = std::make_unique<Texture2D>(m_displayFramebuffer.depthTexture(), TextureType::Depth);
	if (m_depthMap == nullptr)
	{
		std::cout << "Failed to create the Texture2D object. \n";
		return false;
	}

	glCheckError();

	// ------------------------------------------------------------------------
	// Setup scene
	if (setupScene() == false)
		return false;
	glCheckError();

	// ------------------------------------------------------------------------

	// Success
	return true;
}

// ----------------------------------------------------------------------------

bool GLFramework::setupScene()
{
	glCheckError();

	std::cout << "Scene setup. \n";

	// Initialize lights
	LightData::getInstance().initialize();
	MaterialData::getInstance().initialize();

	// Load shaders
	m_basicShader.addShader(Shader::ShaderType::VERTEX, "../Shaders/basic.vert");
	m_basicShader.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/basic.frag");
	if (m_basicShader.initialize() == false) return false;
	
	m_finalShader.addShader(Shader::ShaderType::VERTEX, "../Shaders/hdr.vert");
	m_finalShader.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/hdr.frag");
	if (m_finalShader.initialize() == false) return false;

	m_phongColorShader.addShader(Shader::ShaderType::VERTEX, "../Shaders/phongColor.vert");
	m_phongColorShader.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/phongColor.frag");
	if (m_phongColorShader.initialize() == false) return false;

	m_phongTextureShader.addShader(Shader::ShaderType::VERTEX, "../Shaders/phongTexture.vert");
	m_phongTextureShader.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/phongTexture.frag");
	if (m_phongTextureShader.initialize() == false) return false;

	m_normalMapping.addShader(Shader::ShaderType::VERTEX, "../Shaders/normalMapping.vert");
	m_normalMapping.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/normalMapping.frag");
	if (m_normalMapping.initialize() == false) return false;

	m_parallaxMapping.addShader(Shader::ShaderType::VERTEX, "../Shaders/parallaxMapping.vert");
	m_parallaxMapping.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/parallaxMapping.frag");
	if (m_parallaxMapping.initialize() == false) return false;

	m_colorPBR.addShader(Shader::ShaderType::VERTEX, "../Shaders/simplePBR.vert");
	m_colorPBR.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/simplePBR.frag");
	if (m_colorPBR.initialize() == false) return false;

	m_pbr.addShader(Shader::ShaderType::VERTEX, "../Shaders/pbr.vert");
	m_pbr.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/pbr.frag");
	if (m_pbr.initialize() == false) return false;

	m_depth.addShader(Shader::ShaderType::VERTEX, "../Shaders/shadowMap.vert");
	m_depth.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/shadowMap.frag");
	if (m_depth.initialize() == false) return false;

	m_skyBox.addShader(Shader::ShaderType::VERTEX, "../Shaders/cubemap.vert");
	m_skyBox.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/cubemap.frag");
	if (m_skyBox.initialize() == false) return false;

	m_gbuffer.addShader(Shader::ShaderType::VERTEX, "../Shaders/gbuffer.vert");
	m_gbuffer.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/gbuffer.frag");
	if (m_gbuffer.initialize() == false) return false;

	m_quadShader.addShader(Shader::ShaderType::VERTEX, "../Shaders/quad.vert");
	m_quadShader.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/quad.frag");
	if (m_quadShader.initialize() == false) return false;

	m_quadDepthShader.addShader(Shader::ShaderType::VERTEX, "../Shaders/quadDepth.vert");
	m_quadDepthShader.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/quadDepth.frag");
	if (m_quadDepthShader.initialize() == false) return false;

	m_deferredLighting.addShader(Shader::ShaderType::VERTEX, "../Shaders/deferredLighting.vert");
	m_deferredLighting.addShader(Shader::ShaderType::FRAGMENT, "../Shaders/deferredLighting.frag");
	if (m_deferredLighting.initialize() == false) return false;

	glCheckError();

	// Load textures
	if ((m_brick1Diffuse = TextureMan::Instance().getTexture("../Assets/Textures/brick1/brick_diffuse.jpg", TextureType::Diffuse1)) == nullptr) return false;
	if ((m_brick1Displacement = TextureMan::Instance().getTexture("../Assets/Textures/brick1/brick_displacement.jpg", TextureType::Displacement)) == nullptr) return false;
	if ((m_brick1Normal = TextureMan::Instance().getTexture("../Assets/Textures/brick1/brick_normal.jpg", TextureType::Normal1)) == nullptr) return false;
	if ((m_brick1Specular = TextureMan::Instance().getTexture("../Assets/Textures/brick1/brick_specular.jpg", TextureType::Specular)) == nullptr) return false;

	if ((m_toyBoxDiffuse = TextureMan::Instance().getTexture("../Assets/Textures/toybox/toy_box_diffuse.png", TextureType::Diffuse1)) == nullptr) return false;
	if ((m_toyBoxDisplacement = TextureMan::Instance().getTexture("../Assets/Textures/toybox/toy_box_disp.png", TextureType::Displacement)) == nullptr) return false;
	if ((m_toyBoxNormal = TextureMan::Instance().getTexture("../Assets/Textures/toybox/toy_box_normal.png", TextureType::Normal1)) == nullptr) return false;

	if ((m_brick1RoughnessPBR = TextureMan::Instance().getTexture("../Assets/Textures/brick1/brick_specular.jpg", TextureType::Roughness)) == nullptr) return false;
	if ((m_brick1MetallnessPBR = TextureMan::Instance().getTexture("../Assets/Textures/brick1/brick_specular.jpg", TextureType::Metalness)) == nullptr) return false;
	if ((m_brick1AmbientOcclusionPBR = TextureMan::Instance().getTexture("../Assets/Textures/brick1/brick_displacement.jpg", TextureType::AmbientOcclusion)) == nullptr) return false;

	if ((m_rustedIronAlbedo = TextureMan::Instance().getTexture("../Assets/Textures/pbr/rusted_iron/albedo.png", TextureType::Diffuse1)) == nullptr) return false;
	if ((m_rustedIronRoughness = TextureMan::Instance().getTexture("../Assets/Textures/pbr/rusted_iron/metallic.png", TextureType::Metalness)) == nullptr) return false;
	if ((m_rustedIronMetallic = TextureMan::Instance().getTexture("../Assets/Textures/pbr/rusted_iron/roughness.png", TextureType::Roughness)) == nullptr) return false;
	if ((m_rustedIronAmbientOcclusion = TextureMan::Instance().getTexture("../Assets/Textures/pbr/rusted_iron/ao.png", TextureType::AmbientOcclusion)) == nullptr) return false;
	if ((m_rustedIronNormal = TextureMan::Instance().getTexture("../Assets/Textures/pbr/rusted_iron/normal.png", TextureType::Normal1)) == nullptr) return false;

	if ((m_goldAlbedo = TextureMan::Instance().getTexture("../Assets/Textures/pbr/gold/albedo.png", TextureType::Diffuse1)) == nullptr) return false;
	if ((m_goldRoughness = TextureMan::Instance().getTexture("../Assets/Textures/pbr/gold/metallic.png", TextureType::Metalness)) == nullptr) return false;
	if ((m_goldMetallic = TextureMan::Instance().getTexture("../Assets/Textures/pbr/gold/roughness.png", TextureType::Roughness)) == nullptr) return false;
	if ((m_goldAmbientOcclusion = TextureMan::Instance().getTexture("../Assets/Textures/pbr/gold/ao.png", TextureType::AmbientOcclusion)) == nullptr) return false;
	if ((m_goldNormal = TextureMan::Instance().getTexture("../Assets/Textures/pbr/gold/normal.png", TextureType::Normal1)) == nullptr) return false;

	// Load cube maps
	std::vector<std::string> facePaths = {
		"../Assets/Textures/skybox2/right.jpg", 
		"../Assets/Textures/skybox2/left.jpg",
		"../Assets/Textures/skybox2/top.jpg",
		"../Assets/Textures/skybox2/bottom.jpg",
		"../Assets/Textures/skybox2/back.jpg",
		"../Assets/Textures/skybox2/front.jpg" };
	m_cubeMap1 = TextureMan::Instance().getTexture("skybox1", facePaths);

	facePaths = {
		"../Assets/Textures/skybox1/right.bmp",
		"../Assets/Textures/skybox1/left.bmp",
		"../Assets/Textures/skybox1/top.bmp",
		"../Assets/Textures/skybox1/bottom.bmp",
		"../Assets/Textures/skybox1/front.bmp",
		"../Assets/Textures/skybox1/back.bmp" };
	m_cubeMap2 = TextureMan::Instance().getTexture("skybox2", facePaths);

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
	MaterialData::getInstance().matRustedIron.displacement = m_toyBoxDisplacement;

	// Gold
	MaterialData::getInstance().matGold.albedo = m_goldAlbedo;
	MaterialData::getInstance().matGold.metallic = m_goldMetallic;
	MaterialData::getInstance().matGold.roughness = m_goldRoughness;
	MaterialData::getInstance().matGold.ao = m_goldAmbientOcclusion;
	MaterialData::getInstance().matGold.normal = m_goldNormal;

	glCheckError();

	// Load objects
	m_planeObject = std::make_unique<Object<VertexPTNT> >("../Assets/plane2.obj");
	m_pointLightObject = std::make_unique<Object<VertexPN> >("../Assets/sphere.obj");

	m_planeObjectDeferred = std::make_unique<Object<VertexPNTT> >("../Assets/plane2.obj");
	m_torusModelDeferred = std::make_unique<Object<VertexPNTT> >("../Assets/torus.obj");

	// Load meshes
	//m_pTorusModel = std::make_unique<Model<VertexPN> >("Assets/torus.obj");
	//m_pMonkeyModel = std::make_unique<Model<VertexPN>>("Assets/mymodel.obj");
	//m_pPlaneModel = std::make_unique<Model<VertexPTNT> >("Assets/plane2.obj");
	//m_pLightModel = std::make_unique<Model<VertexPN> >("Assets/sphere.obj");
	//m_pSphereModel = std::make_unique<Model<VertexPTNT> >("Assets/planet.obj");
	//m_bunny = std::make_unique<Model<VertexPTNT> >("Assets/bunny.obj");
	//m_dragon = std::make_unique<Model<VertexPTNT>>("Assets/dragon.obj");
	//m_buddha = std::make_unique<Model<VertexPTNT>>("Assets/buddha.obj");
	//m_lucy = std::make_unique<Model<VertexPTNT>>("Assets/lucy.obj");
	//m_armadillo = std::make_unique<Model<VertexPTNT>>("Assets/armadillo.obj");
	//m_tyra = std::make_unique<Model<VertexPTNT>>("Assets/tyra.obj");
	//m_chair = std::make_unique<Model<VertexPTNT>>("Assets/chair.obj");

	glCheckError();

	// Generate VAO
	m_cubeVAO = Mesh<int>::vaoCubeSetup();
	m_quadVAO = Mesh<int>::vaoQuadSetup();
	m_skyboxVAO = Mesh<int>::vaoSkyboxSetup();

	// Generate and bind vertex array object (Required for OpenGL context > 3.1)
	GLuint vertexArrayObject = 0;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glCheckError();

	// Success
	return true;
}

void GLFramework::drawScene(double dt)
{
	// Do rendering
	glm::mat4 v, p;

	// Get lights
	auto& pointLight0 = LightData::getInstance().pointLight(0);
	//auto& directionalLight0 = LightData::getInstance().directionalLight(0);
	auto& directionalLight1 = LightData::getInstance().directionalLight(1);

	// ------

	// ------------------------------------------------------------------------
	// Render Phong
	m_phongColorShader.useShader();
	// Setup lighting
	m_phongColorShader.set<glm::vec3>(ShaderUniform::LightColor, WHITE);
	m_phongColorShader.set<glm::vec3>(ShaderUniform::LightDir, directionalLight1.direction);
	// Set uniforms
	m_phongColorShader.set<glm::vec4>(ShaderUniform::ObjectColor, WHITE);
	m_phongColorShader.setScalar<float>(ShaderUniform::Shininess, m_pGUI->m_shininess);
	m_phongColorShader.setScalar<float>(ShaderUniform::SpecularStrength, m_pGUI->m_specularStrength);
	// Draw point light sphere
	m_pointLightObject->transform()
		.setPos(pointLight0.position)
		.setScale(glm::vec3(0.01f))
		.setRotation(m_pGUI->m_rotation);
	m_pointLightObject->update(dt);
	m_pointLightObject->render(m_phongColorShader);

	// ------------------------------------------------------------------------
	// Render PBR
	m_pbr.useShader();
	// Setup lighting
	m_pbr.updatePointLights();
	m_pbr.updateDirectionalLights();
	m_pbr.updateSpotLights();
	// Set uniforms
	m_depthMap->bind(m_pbr.program());
	MaterialData::getInstance().matRustedIron.bindTextures(m_pbr.program());
	m_pbr.set<glm::vec2>(ShaderUniform::TextureOffset, m_pGUI->m_textureOffset);
	m_pbr.set<glm::vec2>(ShaderUniform::TextureTile, m_pGUI->m_textureTile);
	m_pbr.setScalar<float>(ShaderUniform::DisplacementMapScale, m_pGUI->m_dispMapScale);
	m_pbr.setScalar<float>(ShaderUniform::NormalMapScale, m_pGUI->m_normalMapScale);
	m_pbr.setScalar<float>(ShaderUniform::Gamma, m_pGUI->m_gamma);
	m_pbr.setScalar<unsigned int>(ShaderUniform::DisplayMode, static_cast<unsigned int>(m_pGUI->m_displayMode));
	// Draw main plane
	m_planeObject->transform()
		.setPos(glm::vec3(0.0f, -1.0f, -2.0f))
		.setScale(glm::vec3(0.5f, 0.001f, 0.5f))
		.setRotation(m_pGUI->m_rotation);
	m_planeObject->update(dt);
	m_planeObject->render(m_pbr);

	// ------------------------------------------------------------------------
	// Render the skybox

	// Set the depth function to less or equal
	glDepthFunc(GL_LEQUAL);
	// Activate the skybox shader
	m_skyBox.useShader();
	// Get rid of the translation component from the view matrix
	// We don't want the skybox to move together with the player
	v = glm::mat4x4(glm::mat3x3(m_cameraMan.getActiveCamera()->viewMatrix()));
	p = m_cameraMan.getActiveCamera()->projMatrix();
	m_skyBox.set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_skyBox.set<glm::mat4>(ShaderUniform::ProjMat, p);
	// Bind the cube VAO
	glBindVertexArray(m_skyboxVAO);
	// Bind the environment texture
	m_cubeMap2->bind(m_skyBox.program());
	// Draw the cube
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Set the depth function to default
	glDepthFunc(GL_LESS);

	// ------------------------------------------------------------------------
}

void GLFramework::drawToGBuffer(double dt)
{
#ifndef NDEBUG
	glCheckError();
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, userEventID, -1, "GBufferPass");
	glCheckError();
#endif // NDEBUG

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// Set the gbuffer as the active framebuffer
	m_gbuffer.useShader();
	m_gbufferFramebuffer.renderToTexture();

	// Set uniforms
	MaterialData::getInstance().matRustedIron.bindTextures(m_gbuffer.program());
	m_gbuffer.set<glm::vec2>(ShaderUniform::TextureOffset, m_pGUI->m_textureOffset);
	m_gbuffer.set<glm::vec2>(ShaderUniform::TextureTile, m_pGUI->m_textureTile);
	m_gbuffer.setScalar<float>(ShaderUniform::DisplacementMapScale, m_pGUI->m_dispMapScale);
	
	// Draw main plane
	m_planeObjectDeferred->transform()
		.setPos(glm::vec3(0.0f, -1.0f, -2.0f))
		.setScale(glm::vec3(0.5f, 0.001f, 0.5f))
		.setRotation(m_pGUI->m_rotation);
	m_planeObjectDeferred->update(dt);
	m_planeObjectDeferred->render(m_gbuffer);

	// Draw deferred torus
	m_torusModelDeferred->transform()
		.setPos(glm::vec3(0.0f, -1.0f, -2.0f))
		.setScale(glm::vec3(1.0f, 1.0f, 1.0f))
		.setRotation(m_pGUI->m_rotation);
	m_torusModelDeferred->update(dt);
	m_torusModelDeferred->render(m_gbuffer);

#ifndef NDEBUG
	glPopDebugGroup();
	glCheckError();
#endif // NDEBUG
}

void GLFramework::drawDeferredLighting(double dt)
{
#ifndef NDEBUG
	glCheckError();
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, userEventID, -1, "DeferredLightingPass");
#endif // NDDEBUG

	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	// Set the display framebuffer as the active framebuffer
	m_displayFramebuffer.renderToTexture();

	// Set uniforms
	m_deferredLighting.useShader();
	// Setup lighting
	m_deferredLighting.updatePointLights();
	m_deferredLighting.updateDirectionalLights();
	m_deferredLighting.updateSpotLights();
	// Set uniforms
	m_depthMap->bind(m_deferredLighting.program());
	
	// Bind gbuffer textures	
	int textureUnitIndex = 0;
	Texture2D::bind(glGetUniformLocation(m_deferredLighting.program(), "gPosition"), m_gbufferFramebuffer.colorTexture("Position"), textureUnitIndex++);
	Texture2D::bind(glGetUniformLocation(m_deferredLighting.program(), "gNormal"), m_gbufferFramebuffer.colorTexture("Normal"), textureUnitIndex++);
	Texture2D::bind(glGetUniformLocation(m_deferredLighting.program(), "gAlbedo"), m_gbufferFramebuffer.colorTexture("Albedo"), textureUnitIndex++);
	Texture2D::bind(glGetUniformLocation(m_deferredLighting.program(), "gPBR"), m_gbufferFramebuffer.colorTexture("PBR"), textureUnitIndex++);

	glCheckError();

	m_deferredLighting.setScalar<float>(ShaderUniform::NormalMapScale, m_pGUI->m_normalMapScale);
	m_deferredLighting.setScalar<float>(ShaderUniform::Gamma, m_pGUI->m_gamma);
	m_deferredLighting.setScalar<unsigned int>(ShaderUniform::DisplayMode, static_cast<unsigned int>(m_pGUI->m_displayMode));
	m_deferredLighting.set<glm::vec3>(ShaderUniform::ViewPos, CameraMan::Instance().getActiveCamera()->viewPos());

	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

#ifndef NDEBUG
	glPopDebugGroup();
	glCheckError();
#endif // NDEBUG
}

void GLFramework::drawForwardLighting(double dt)
{
#ifndef NDEBUG
	glCheckError();
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, userEventID, -1, "ForwardLightingPass");
#endif // NDEBUG

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// Set the display framebuffer as the active framebuffer
	m_displayFramebuffer.renderToTexture(Framebuffer::RenderTargetType::COLOR_TARGET, false);

	// ------------------------------------------------------------------------

	// Render Phong
	m_phongColorShader.useShader();
	// Setup lighting
	m_phongColorShader.updatePointLights();
	m_phongColorShader.updateDirectionalLights();
	m_phongColorShader.updateSpotLights();
	// Set uniforms
	m_phongColorShader.set<glm::vec4>(ShaderUniform::ObjectColor, WHITE);
	m_phongColorShader.setScalar<float>(ShaderUniform::Shininess, m_pGUI->m_shininess);
	m_phongColorShader.setScalar<float>(ShaderUniform::SpecularStrength, m_pGUI->m_specularStrength);
	// Draw point light sphere
	m_pointLightObject->transform()
		.setPos(LightData::getInstance().pointLight(0).position)
		.setScale(glm::vec3(0.01f))
		.setRotation(m_pGUI->m_rotation);
	m_pointLightObject->update(dt);
	m_pointLightObject->render(m_phongColorShader);

	// ------------------------------------------------------------------------

	// Render the skybox

	// Set the depth function to less or equal
	glDepthFunc(GL_LEQUAL);
	// Activate the skybox shader
	m_skyBox.useShader();
	// Get rid of the translation component from the view matrix
	// We don't want the skybox to move together with the player
	glm::mat4 v = glm::mat4x4(glm::mat3x3(m_cameraMan.getActiveCamera()->viewMatrix()));
	glm::mat4 p = m_cameraMan.getActiveCamera()->projMatrix();
	m_skyBox.set<glm::mat4>(ShaderUniform::ViewMat, v);
	m_skyBox.set<glm::mat4>(ShaderUniform::ProjMat, p);
	// Bind the cube VAO
	glBindVertexArray(m_skyboxVAO);
	// Bind the environment texture
	m_cubeMap2->bind(m_skyBox.program());
	// Draw the cube
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// ------------------------------------------------------------------------

#ifndef NDEBUG
	glPopDebugGroup();
	glCheckError();
#endif // NDEBUG
}

void GLFramework::drawSceneToDepth()
{
	// Configure shader and matrices
	float nearPlane = 1.0f, farPlane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f,
		-10.0f, 10.0f,
		nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(
		-LightData::getInstance().directionalLight(0).direction,
		glm::vec3(0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)); // Up vector ?????
	glm::mat4 lightMatrix = lightProjection * lightView;

	glm::mat4 m;

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
	// Activate shader
	m_depth.useShader();
	// Set uniforms
	m_depth.set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_depth.set<glm::mat4>(ShaderUniform::LightMat, lightMatrix);
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
	// Activate shader
	m_depth.useShader();
	// Matrices
	m_depth.set<glm::mat4>(ShaderUniform::ModelMat, m);
	m_depth.set<glm::mat4>(ShaderUniform::LightMat, lightMatrix);
	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//// ---------------------------------------------------------------------
	//// Draw sphere rusted iron

	//// Set matrices
	//scaleFactor = 0.02f;
	//position = glm::vec3(-5.0f, 0.0f, -2.0f);
	//m = glm::mat4();
	//m = glm::scale(m, glm::vec3(scaleFactor));
	//m = glm::translate(m, position);
	//rotMat = glm::toMat4(m_pGUI->m_rotation);
	//m = m * rotMat;
	//// Activate shader
	//m_depth.useShader();
	//// Matrices
	//m_depth.set<glm::mat4>(ShaderUniform::ModelMat, m);
	//m_depth.set<glm::mat4>(ShaderUniform::LightMat, lightMatrix);
	//// Draw triangles
	//m_pSphereModel->render();

	//// ---------------------------------------------------------------------
	//// Draw sphere gold

	//// Set matrices
	//scaleFactor = 0.02f;
	//position = glm::vec3(-10.0f, 0.0f, -2.0f);
	//m = glm::mat4();
	//m = glm::scale(m, glm::vec3(scaleFactor));
	//m = glm::translate(m, position);
	//rotMat = glm::toMat4(m_pGUI->m_rotation);
	//m = m * rotMat;
	//// Activate shader
	//m_depth.useShader();
	//// Matrices
	//m_depth.set<glm::mat4>(ShaderUniform::ModelMat, m);
	//m_depth.set<glm::mat4>(ShaderUniform::LightMat, lightMatrix);
	//// Draw triangles
	//m_pSphereModel->render();

	//// ------------------------------------------------------------------------
	//// Draw torus - simple color PBR

	//// Set matrices
	//scaleFactor = 0.1f;
	//position = glm::vec3(1.0f, 0.0f, -3.0f);
	//m = glm::mat4();
	//m = glm::scale(m, glm::vec3(scaleFactor));
	//m = glm::translate(m, position);
	//rotMat = glm::toMat4(m_pGUI->m_rotation);
	//m = m * rotMat;
	//// Activate shader
	//m_depth.useShader();
	//// Matrices
	//m_depth.set<glm::mat4>(ShaderUniform::ModelMat, m);
	//m_depth.set<glm::mat4>(ShaderUniform::LightMat, lightMatrix);
	//// Draw triangles
	//m_pTorusModel->render();

	//// ------------------------------------------------------
	//// Draw quad - parallax mapping

	//position = glm::vec3(0.0f, -1.0f, -2.0f);
	//scaleFactor = 0.1f;
	//m = glm::mat4();
	//m = glm::translate(m, position);
	//m = glm::rotate(m, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//m = glm::scale(m, glm::vec3(scaleFactor, 0.001f, scaleFactor));
	//// Activate shader
	//m_depth.useShader();
	//// Matrices
	//m_depth.set<glm::mat4>(ShaderUniform::ModelMat, m);
	//m_depth.set<glm::mat4>(ShaderUniform::LightMat, lightMatrix);
	//// Draw triangles
	//m_pPlaneModel->render();
}

// ----------------------------------------------------------------------------
