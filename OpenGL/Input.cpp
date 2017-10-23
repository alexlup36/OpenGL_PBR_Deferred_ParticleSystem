#include "Input.h"

#include <AntTweakBar.h>

// ----------------------------------------------------------------------------

float Input::m_fCameraSpeed = 2.5f;

// ----------------------------------------------------------------------------

void Input::initialize(GLFWwindow* window)
{
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)Input::onMouseClicked);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)Input::onMouseMoved);
	glfwSetScrollCallback(window, (GLFWscrollfun)Input::onScrollCallback);
	glfwSetKeyCallback(window, (GLFWkeyfun)Input::onKeyPressed);
	glfwSetWindowSizeCallback(window, (GLFWwindowsizefun)Input::onResize);
}

// ----------------------------------------------------------------------------

void Input::onMouseMoved(GLFWwindow* window, double x, double y)
{
	TwMouseMotion(static_cast<int>(x), static_cast<int>(y));
}

// ----------------------------------------------------------------------------

void Input::onMouseClicked(GLFWwindow* window, int button, int action, int mods)
{
	TwMouseButtonID btn = (button == 0) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
	TwMouseAction ma = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	TwMouseButton(ma, btn);
}

// ----------------------------------------------------------------------------

void Input::onKeyPressed(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_LEFT: key = TW_KEY_LEFT; break;
	case GLFW_KEY_RIGHT: key = TW_KEY_RIGHT; break;
	case GLFW_KEY_UP: key = TW_KEY_UP; break;
	case GLFW_KEY_DOWN: key = TW_KEY_DOWN; break;
	default: break;
	}

	int tw_mod = TW_KMOD_NONE;

	if (mods & GLFW_MOD_SHIFT) {
		tw_mod |= TW_KMOD_SHIFT;
	}

	if (mods & GLFW_MOD_CONTROL) {
		tw_mod |= TW_KMOD_CTRL;
	}

	if (mods & GLFW_MOD_ALT) {
		tw_mod |= TW_KMOD_ALT;
	}

	TwKeyPressed(key, TW_KMOD_NONE);
}

// ----------------------------------------------------------------------------

void Input::onResize(GLFWwindow* window, int w, int h)
{
	TwWindowSize(w, h);
}

// ----------------------------------------------------------------------------

void Input::onScrollCallback(GLFWwindow* window, double x, double y)
{
	TwMouseWheel(static_cast<int>(y));
}

// ----------------------------------------------------------------------------