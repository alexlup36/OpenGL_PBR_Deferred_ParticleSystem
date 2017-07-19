#include <assert.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include "GUI.h"

GUI::GUI()
	:win_w(0), win_h(0)
{
}

GUI::~GUI()
{
	TwTerminate();
}

bool GUI::setup(GLFWwindow* window, int w, int h) 
{
	assert(w);
	assert(h);
	assert(window);
	win_w = w;
	win_h = h;

	TwInit(TW_OPENGL_CORE, NULL);

	bar = TwNewBar("Phong Lighting");

	TwAddVarRW(bar, "ObjectShininess", TW_TYPE_FLOAT, &m_shininess, "min=0 max=256 step=1.0 help='Power of 2'");

	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, m_rotation,
		" label='Object rotation' opened=true help='Change the object orientation.' ");

	TwAddVarRW(bar, "LightDirection", TW_TYPE_DIR3F, m_lightDirection,
		" label='Light direction' opened=true help='Change the light direction.' ");

	TwAddVarRW(bar, "ObjectColor", TW_TYPE_COLOR4F, m_objectColor,
		" label='Cube color' alpha help='Color and transparency of the cube.' ");

	TwAddVarRW(bar, "LightColor", TW_TYPE_COLOR3F, m_lightColor, " label='Light color' ");

	TwWindowSize(win_w, win_h);

	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventCursorPosGLFW3);
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventScrollGLFW3);
	glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW3);
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);

	return true;
}

void GUI::onResize(int w, int h) 
{
	win_w = w;
	win_h = h;
	TwWindowSize(w, h);
}

void GUI::onMouseMoved(double x, double y)
{
	TwMouseMotion(static_cast<int>(x), static_cast<int>(y));
}

void GUI::onMouseClicked(int bt, int action) 
{
	TwMouseButtonID btn = (bt == 0) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
	TwMouseAction ma = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	TwMouseButton(ma, btn);
}

void GUI::onKeyPressed(int key, int mod) 
{

	switch (key) {
	case GLFW_KEY_LEFT: key = TW_KEY_LEFT; break;
	case GLFW_KEY_RIGHT: key = TW_KEY_RIGHT; break;
	case GLFW_KEY_UP: key = TW_KEY_UP; break;
	case GLFW_KEY_DOWN: key = TW_KEY_DOWN; break;
	default: break;
	}

	int tw_mod = TW_KMOD_NONE;

	if (mod & GLFW_MOD_SHIFT) {
		tw_mod |= TW_KMOD_SHIFT;
	}

	if (mod & GLFW_MOD_CONTROL) {
		tw_mod |= TW_KMOD_CTRL;
	}

	if (mod & GLFW_MOD_ALT) {
		tw_mod |= TW_KMOD_ALT;
	}

	TwKeyPressed(key, TW_KMOD_NONE);
}

void GUI::draw() 
{
	TwDraw();

	//std::cout << std::endl;
	//std::cout << "Axis: x: " << m_rotation[0] << " y: " << m_rotation[1] << " z: " << m_rotation[2] << std::endl;
	//std::cout << "Angle: " << m_rotation[3] << std::endl;
}