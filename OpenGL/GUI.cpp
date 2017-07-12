#include <assert.h>
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
	bar = TwNewBar("SWNT");
	TwAddVarRW(bar, "speed", TW_TYPE_DOUBLE, &test, "min=0 max=10 step=0.1 help='test'");

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
}