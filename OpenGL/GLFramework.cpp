#include "GLFramework.h"

#include "GUI.h"
#include "Input.h"
#include "Camera.h"

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
	// Call base class
	OpenGLApp::draw(dt);

	// ------------------------------------------------------------------------
	// Do drawing here


	// ------------------------------------------------------------------------
	// Draw GUI
	m_pGUI->draw();

	// Swap buffers
	glfwSwapBuffers(window());
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
}

// ----------------------------------------------------------------------------
