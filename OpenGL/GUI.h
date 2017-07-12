#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <AntTweakBar.h>

class GUI 
{
public:
	GUI();
	~GUI();
	bool setup(GLFWwindow* window, int w, int h);
	void onMouseMoved(double x, double y);
	void onMouseClicked(int bt, int action);
	void onKeyPressed(int key, int mod);
	void onResize(int w, int h);
	void draw();
public:
	int win_w;
	int win_h;
	TwBar* bar;
	double test;
};

#endif // GUI_H