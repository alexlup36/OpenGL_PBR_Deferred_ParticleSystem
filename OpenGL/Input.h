#ifndef INPUT_H
#define INPUT_H

#include "Common.h"

class Input
{
public:
	static Input& getInstance()
	{
		static Input instance;
		return instance;
	}

	void initialize(GLFWwindow* window);
	static void onMouseMoved(GLFWwindow* window, double x, double y);
	static void onMouseClicked(GLFWwindow* window, int button, int action, int mods);
	static void onKeyPressed(GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void onScrollCallback(GLFWwindow* window, double x, double y);
	static void onResize(GLFWwindow* window, int w, int h);

private:
	Input() {}

public:
	Input(Input const&) = delete;
	void operator=(Input const&) = delete;
};

#endif // INPUT_H