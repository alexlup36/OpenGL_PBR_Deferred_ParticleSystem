#ifndef WINDOW_H
#define WINDOW_H

#include "Common.h"

struct Window
{
	GLFWwindow* window;
	int windowWidth, windowHeight;
};

#endif // WINDOW_H