#include "Input.h"

//#include <AntTweakBar.h>

// ----------------------------------------------------------------------------

float Input::m_fCameraSpeed = 1.5f;
float Input::m_fMovementSpeed = 0.5f;
bool Input::m_bFPSCameraEnabled = true;

// ----------------------------------------------------------------------------

void Input::initialize(GLFWwindow* window)
{
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)Input::onMouseClicked);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)Input::onMouseMoved);
	glfwSetScrollCallback(window, (GLFWscrollfun)Input::onScrollCallback);
	glfwSetKeyCallback(window, (GLFWkeyfun)Input::onKeyPressed);
	glfwSetWindowSizeCallback(window, (GLFWwindowsizefun)Input::onResize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// ----------------------------------------------------------------------------

void Input::onMouseMoved(GLFWwindow* window, double x, double y)
{
}

// ----------------------------------------------------------------------------

void Input::onMouseClicked(GLFWwindow* window, int button, int action, int mods)
{
}

// ----------------------------------------------------------------------------

void Input::onKeyPressed(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		m_bFPSCameraEnabled = !m_bFPSCameraEnabled;
		if (m_bFPSCameraEnabled)
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			glfwSetCursorPos(window, width * 0.5f, height * 0.5f);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		return;
	}
}

// ----------------------------------------------------------------------------

void Input::onResize(GLFWwindow* window, int w, int h)
{
}

// ----------------------------------------------------------------------------

void Input::onScrollCallback(GLFWwindow* window, double x, double y)
{
}

// ----------------------------------------------------------------------------