#ifndef GLFRAMEWORK_H
#define GLFRAMEWORK_H

#include "OpenGLApp.h"
#include <memory>

class GUI;
class Camera;

class GLFramework : public OpenGLApp
{
public:
	GLFramework(int windowWidth, int windowHeight);
	~GLFramework();

	virtual void update(double dt);
	virtual void draw(double dt);

	virtual bool initialize(const char* windowTitle, bool enableMultisampling, bool enableSRGB);

private:
	virtual void setupScene();

	// User interface manager
	GUI* m_pGUI;
	// FPS camera
	std::unique_ptr<Camera> m_pCamera1;
};

#endif // GLFRAMEWORK_H