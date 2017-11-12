#include <assert.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include "GUI.h"

GUI::GUI()
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

	TwInit(TW_OPENGL_CORE, NULL);

	bar = TwNewBar("Phong Lighting");

	TwAddVarRW(bar, "ObjectShininess", TW_TYPE_FLOAT, &m_shininess, "min=1 max=256 step=0.1 help='Power of 2'");
	TwAddVarRW(bar, "ObjectSpecularStrength", TW_TYPE_FLOAT, &m_specularStrength, "min=0 max=1 step=0.01 ");

	//TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, m_rotation,
	//	" label='Object rotation' opened=true help='Change the object orientation.' ");

	//TwAddVarRW(bar, "LightDirection", TW_TYPE_DIR3F, m_lightDirection,
	//	" label='Light direction' opened=true help='Change the light direction.' ");

	//TwAddVarRW(bar, "ObjectColor", TW_TYPE_COLOR4F, m_objectColor,
	//	" label='Cube color' alpha help='Color and transparency of the cube.' ");

	//TwAddVarRW(bar, "LightColor", TW_TYPE_COLOR3F, m_lightColor, " label='Light color' ");

	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, static_cast<void*>(&m_rotation[0]),
		" label='Object rotation' opened=true help='Change the object orientation.' ");

	TwAddVarRW(bar, "LightDirection", TW_TYPE_DIR3F, static_cast<void*>(&m_lightDirection[0]),
		" label='Light direction' opened=true help='Change the light direction.' ");

	TwAddVarRW(bar, "ObjectColor", TW_TYPE_COLOR4F, static_cast<void*>(&m_objectColor[0]),
		" label='Cube color' alpha help='Color and transparency of the cube.' ");

	TwAddVarRW(bar, "LightColor", TW_TYPE_COLOR3F, static_cast<void*>(&m_lightColor[0]), " label='Light color' ");

	TwWindowSize(w, h);

	return true;
}

void GUI::draw() 
{
	TwDraw();
}