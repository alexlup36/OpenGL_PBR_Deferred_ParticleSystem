#include "GLFramework.h"

#include "glm\glm.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

#include "GUI.h"
#include "Input.h"
#include "Camera.h"
#include "Shader.h"

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
	m_pCamera1->processInput(window(), dt);
	m_pCamera1->updateView();

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
	glm::mat4 m, p, mvp, normalMat;

	// ------------------------------------------------------------------------
	// Draw cube

	// Set matrices
	glBindVertexArray(m_cubeVAO);
	float scaleFactor = 0.04f;
	glm::vec3 position = glm::vec3(-1.0f, 0.0f, -2.0f);
	m = glm::mat4();
	m = glm::scale(m, glm::vec3(scaleFactor));
	m = glm::translate(m, position);
	glm::quat quat = glm::quat(m_pGUI->m_rotation[3], m_pGUI->m_rotation[0], m_pGUI->m_rotation[1], m_pGUI->m_rotation[2]);
	glm::mat4 rotMat = glm::toMat4(quat);
	m = m * rotMat;
	normalMat = glm::transpose(glm::inverse(m));
	p = glm::perspective(glm::radians(60.0f), static_cast<float>(windowWidth()) / windowHeight(), 0.01f, 10000.0f);
	glm::mat4 v = glm::mat4(1.0f); // TODO - needs to be replaced with the camera view matrix
	// Activate shader
	m_phongColorShader->useShader();
	// Set uniforms
	m_phongColorShader->set<glm::mat4&>(ShaderUniform::ModelMat, m);
	m_phongColorShader->set<glm::mat4&>(ShaderUniform::NormalMat, normalMat);
	m_phongColorShader->set<glm::mat4&>(ShaderUniform::ViewMat, v);
	m_phongColorShader->set<glm::mat4&>(ShaderUniform::ProjMat, p);
	m_phongColorShader->set<glm::vec3&>(ShaderUniform::LightColor, m_pGUI->m_lightColor);
	m_phongColorShader->set<glm::vec3&>(ShaderUniform::LightDir, m_pGUI->m_lightDirection);
	m_phongColorShader->set<glm::vec4&>(ShaderUniform::ObjectColor, m_pGUI->m_objectColor);
	// TODO - replace with the camera position
	m_phongColorShader->set<glm::vec3&>(ShaderUniform::ViewPos, glm::vec3(0.0f, 0.0f, 0.0f));
	m_phongColorShader->set<float>(ShaderUniform::Shininess, m_pGUI->m_shininess);
	m_phongColorShader->set<float>(ShaderUniform::SpecularStrength, m_pGUI->m_specularStrength);
	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// ------------------------------------------------------------------------
	// Draw torus

	// Set matrices
	scaleFactor = 0.1f;
	position = glm::vec3(1.0f, 0.0f, -3.0f);
	m = glm::mat4();
	m = glm::scale(m, glm::vec3(scaleFactor));
	m = glm::translate(m, position);
	quat = glm::quat(m_pGUI->m_rotation[3], m_pGUI->m_rotation[0], m_pGUI->m_rotation[1], m_pGUI->m_rotation[2]);
	rotMat = glm::toMat4(quat);
	m = m * rotMat;
	normalMat = glm::transpose(glm::inverse(m));
	p = glm::perspective(glm::radians(60.0f), static_cast<float>(windowWidth()) / windowHeight(), 0.01f, 10000.0f);
	v = glm::mat4(1.0f); // TODO - needs to be replaced with the camera view matrix
	// Activate shader
	m_phongColorShader->useShader();
	// Set uniforms
	m_phongColorShader->set<glm::mat4&>(ShaderUniform::ModelMat, m);
	m_phongColorShader->set<glm::mat4&>(ShaderUniform::NormalMat, normalMat);
	m_phongColorShader->set<glm::mat4&>(ShaderUniform::ViewMat, v);
	m_phongColorShader->set<glm::mat4&>(ShaderUniform::ProjMat, p);
	m_phongColorShader->set<glm::vec3&>(ShaderUniform::LightColor, m_pGUI->m_lightColor);
	m_phongColorShader->set<glm::vec3&>(ShaderUniform::LightDir, m_pGUI->m_lightDirection);
	m_phongColorShader->set<glm::vec4&>(ShaderUniform::ObjectColor, m_pGUI->m_objectColor);
	// TODO - replace with the camera position
	m_phongColorShader->set<glm::vec3&>(ShaderUniform::ViewPos, glm::vec3(0.0f, 0.0f, 0.0f));
	m_phongColorShader->set<float>(ShaderUniform::Shininess, m_pGUI->m_shininess);
	m_phongColorShader->set<float>(ShaderUniform::SpecularStrength, m_pGUI->m_specularStrength);
	// Draw triangles
	m_pTorusModel->render();

	// ------------------------------------------------------
	// Draw quad
	position = glm::vec3(0.0f, -1.0f, -2.0f);
	scaleFactor = 1.0f;
	m = glm::mat4();
	m = glm::translate(m, position);
	m = glm::rotate(m, 20.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, glm::vec3(scaleFactor, 1.0f, scaleFactor));
	normalMat = glm::transpose(glm::inverse(m));
	// Activate shader
	m_phongTextureShader->useShader();
	// Set uniforms
	m_phongTextureShader->set<glm::mat4&>(ShaderUniform::ModelMat, m);
	m_phongTextureShader->set<glm::mat4&>(ShaderUniform::NormalMat, normalMat);
	m_phongTextureShader->set<glm::mat4&>(ShaderUniform::ViewMat, v);
	m_phongTextureShader->set<glm::mat4&>(ShaderUniform::ProjMat, p);
	m_phongTextureShader->set<glm::vec3&>(ShaderUniform::LightColor, m_pGUI->m_lightColor);
	m_phongTextureShader->set<glm::vec3&>(ShaderUniform::LightDir, m_pGUI->m_lightDirection);
	glm::vec4 quadColor = glm::vec4(0.2f, 0.6f, 0.9f, 1.0f);
	m_phongTextureShader->set<glm::vec4&>(ShaderUniform::ObjectColor, quadColor);
	// TODO - replace with the camera position
	m_phongTextureShader->set<glm::vec3&>(ShaderUniform::ViewPos, glm::vec3(0.0f, 0.0f, 0.0f));
	m_phongTextureShader->set<float>(ShaderUniform::Shininess, m_pGUI->m_shininess);
	m_brick1Diffuse->bind(m_phongTextureShader->program());
	m_brick1Displacement->bind(m_phongTextureShader->program());
	m_brick1Normal->bind(m_phongTextureShader->program());
	m_brick1Specular->bind(m_phongTextureShader->program());
	// Draw triangles
	m_pPlaneModel->render();

	// Render to screen
	// Activate shader
	glBindVertexArray(m_quadVAO);
	m_quadShader->useShader();
	GLuint textureUnit = 0;
	m_pRT->renderToScreen(0, 0, windowWidth(), windowHeight(), textureUnit);
	m_quadShader->set<int>(ShaderUniform::RenderedTexture, textureUnit);
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
	m_pCamera1 = std::make_unique<Camera>(windowWidth(), windowHeight());

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

	// Load shaders
	m_basicShader = std::make_unique<Shader>(".\\Shaders\\basic.vert", ".\\Shaders\\basic.frag");
	m_quadShader = std::make_unique<Shader>(".\\Shaders\\quad.vert", ".\\Shaders\\quad.frag");
	m_phongColorShader = std::make_unique<Shader>(".\\Shaders\\phongColor.vert", ".\\Shaders\\phongColor.frag");
	m_phongTextureShader = std::make_unique<Shader>(".\\Shaders\\phongTexture.vert", ".\\Shaders\\phongTexture.frag");
	m_normalMapping = std::make_unique<Shader>(".\\Shaders\\normalMapping.vert", ".\\Shaders\\normalMapping.frag");
	m_parallaxMapping = std::make_unique<Shader>(".\\Shaders\\parallaxMapping.vert", ".\\Shaders\\parallaxMapping.frag");
	m_simplePBR = std::make_unique<Shader>(".\\Shaders\\simplePBR.vert", ".\\Shaders\\simplePBR.frag");
	m_pbr = std::make_unique<Shader>(".\\Shaders\\pbr.vert", ".\\Shaders\\pbr.frag");

	// Load textures
	m_brick1Diffuse = TextureMan::Instance().getTexture("..//Assets//Textures//brick1//brick_diffuse.jpg", TextureType::Diffuse1);
	m_brick1Displacement = TextureMan::Instance().getTexture("..//Assets//Textures//brick1/brick_displacement.jpg", TextureType::Displacement);
	m_brick1Normal = TextureMan::Instance().getTexture("..//Assets//Textures//brick1//brick_normal.jpg", TextureType::Normal1);
	m_brick1Specular = TextureMan::Instance().getTexture("..//Assets//Textures//brick1//brick_specular.jpg", TextureType::Specular);

	// Load meshes
	m_pTorusModel = std::make_unique<Model<VertexPN>>("..//Assets//torus.obj");
	//m_pMonkeyModel = std::make_unique<Model<VertexPN>>("..//Assets//mymodel.obj");
	m_pPlaneModel = std::make_unique<Model<VertexPTNT>>("..//Assets//plane.obj");

	// Generate VAO
	m_cubeVAO = Mesh<int>::vaoCubeSetup();
	m_quadVAO = Mesh<int>::vaoQuadSetup();

	// Generate and bind vertex array object (Required for OpenGL context > 3.1)
	GLuint vertexArrayObject = 0;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
}

// ----------------------------------------------------------------------------
