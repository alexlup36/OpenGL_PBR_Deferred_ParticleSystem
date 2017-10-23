#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <AntTweakBar.h>
#include <glm/gtc/quaternion.hpp>

class GUI 
{
public:
	GUI();
	~GUI();
	bool setup(GLFWwindow* window, int w, int h);
	void draw();
public:
	TwBar* bar;
	double test;
	float m_rotation[4] = { 0.5f, 0.5f, 0.5f };
	float m_lightDirection[3] = { 0.5f, 0.5f, 0.5f };
	float m_shininess = 32.0f;
	float m_lightColor[3] = { 1.0f, 1.0f, 1.0f };
	float m_objectColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
};

#endif // GUI_H