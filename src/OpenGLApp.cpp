#include "OpenGLApp.h"
#include <iostream>
#include <vector>
#include <assert.h>

// ----------------------------------------------------------------------------

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// ----------------------------------------------------------------------------

OpenGLApp::OpenGLApp(int windowWidth, int windowHeight)
	: m_iWindowWidth(windowWidth), m_iWindowHeight(windowHeight)
{
}

OpenGLApp::~OpenGLApp()
{
	// Should be done last

	// Destroy window
	if (m_pWindow)
		glfwDestroyWindow(m_pWindow);
	// Shutdown GLFW
	glfwTerminate();
}

// ----------------------------------------------------------------------------

bool OpenGLApp::initialize(const char* windowTitle, 
	bool enableMultisampling,
	bool enableSRGB)
{
	std::cout << "OpenGLApp initialize. \n";

	assert(windowTitle != nullptr && "Invalid window title.");

	// Set GLFW error callback method
	glfwSetErrorCallback(error_callback);

	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW. \n";
		return false;
	}

	// Set GLFW context version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Set opengl profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

	// Set window properties
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Enable multisampling
	if (enableMultisampling)
		glfwWindowHint(GLFW_SAMPLES, 4);

	// Enable SRGB framebuffer support
	if (enableSRGB)
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	// Create a debug context for a debug build
#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif // DEBUG

	// Create window
	m_pWindow = glfwCreateWindow(m_iWindowWidth, m_iWindowHeight, windowTitle, NULL, NULL);
	if (!m_pWindow)
	{
		std::cout << "Failed to create GLFW window " << windowTitle << "\n";
		glfwTerminate();
		return false;
	}

	//glfwSetKeyCallback(window, key_callback);

	// Activate current context
	glfwMakeContextCurrent(m_pWindow);
	glewExperimental = true;

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW\n";
		return false;
	}

	std::vector<std::string> openGLVersions = {
		"GL_VERSION_1_4",
		"GL_VERSION_3_3",
		"GL_VERSION_4_0",
		"GL_VERSION_4_1",
		"GL_VERSION_4_2",
		"GL_VERSION_4_3",
		"GL_VERSION_4_4",
		"GL_VERSION_4_5",
		"GL_VERSION_4_6"
	};

	for (auto it = openGLVersions.rbegin(); it != openGLVersions.rend(); ++it) 
	{
		if (glewIsSupported((*it).c_str()))
		{
			std::cout << "OpenGL " << *it << " is supported.\n";
			break;
		}
	}

	// Set swap interval - enable VSync by default
	glfwSwapInterval(1);

	// Set default opengl state
	setOpenGLState();

	// Success
	return true;
}

// ----------------------------------------------------------------------------

void OpenGLApp::update(double dt)
{
	glfwPollEvents();
}

// ----------------------------------------------------------------------------

void OpenGLApp::draw(double dt)
{
}

// ----------------------------------------------------------------------------

bool OpenGLApp::setupScene()
{
	// Success
	return true;
}

// ----------------------------------------------------------------------------

bool OpenGLApp::setOpenGLState() const
{
#ifndef NDEBUG
	GLint flags;
	// Get context information
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	// If we have a debug context created
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// Enable OpenGL debug output
		glEnable(GL_DEBUG_OUTPUT);
		// Enable synchronous output
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// Set the callback method for debugging
		glDebugMessageCallback(glDebugOutput, nullptr);
		// Option to filter the messages we get
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	else
	{
		// This is a debug build but for some reason we don't have a debug context
		std::cout << "Failed to create a OpenGL debug context.\n";
		return false;
	}
#endif // DEBUG

	// Set clear color
	glClearColor(0.2f, 0.1f, 0.3f, 1.0f);

	// Enable face culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// Enable depth test and set depth function
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// Success
	return true;
}

// ----------------------------------------------------------------------------