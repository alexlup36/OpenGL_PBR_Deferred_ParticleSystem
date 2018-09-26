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

	// Getters/Setters
	inline GLFWwindow* window() const { return m_pWindow; }
	inline int windowWidth() const { return m_iWindowWidth; }
	inline int windowHeight() const { return m_iWindowHeight; }

protected:
	virtual void update(double dt);
	virtual void draw(double dt);

	virtual bool initialize(const char* windowTitle, bool enableMultisampling, bool enableSRGB);
	virtual bool setupScene();

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