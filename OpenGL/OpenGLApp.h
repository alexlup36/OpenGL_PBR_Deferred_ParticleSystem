#ifndef OPENGLAPP_H
#define OPENGLAPP_H

// ----------------------------------------------------------------------------

#include "Common.h"

// ----------------------------------------------------------------------------

class OpenGLApp
{
public:
	OpenGLApp(int windowWidth, int windowHeight);
	~OpenGLApp();

	void run();

	// Getters/Setters
	inline const GLFWwindow* window() const { return m_pWindow; }

protected:
	virtual void update(float dt);
	virtual void draw(float dt);

	virtual bool initialize(const char* windowTitle, bool enableMultisampling, bool enableSRGB);
	virtual void setupScene();

private:
	GLFWwindow* m_pWindow;

	// Window size
	int m_iWindowWidth;
	int m_iWindowHeight;

	// Set OpenGL state
	bool setOpenGLState() const;
};

// ----------------------------------------------------------------------------

#endif // OPENGLAPP_H