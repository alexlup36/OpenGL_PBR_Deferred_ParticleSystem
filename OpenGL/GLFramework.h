#ifndef GLFRAMEWORK_H
#define GLFRAMEWORK_H

#include "OpenGLApp.h"
#include "RenderTarget.h"
#include <memory>
#include "Mesh.h"

class GUI;
class Camera;
class Shader;

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

	// VAO
	GLuint m_cubeVAO;
	GLuint m_quadVAO;

	// Shaders
	std::unique_ptr<Shader> m_basicShader = nullptr;
	std::unique_ptr<Shader> m_quadShader = nullptr;
	std::unique_ptr<Shader> m_phongShader = nullptr;

	// FPS camera
	std::unique_ptr<Camera> m_pCamera1;
	// Render target
	std::unique_ptr<RenderTarget> m_pRT;

	std::unique_ptr<Mesh<VertexPTNT>> m_pNanosuitMesh;
};

#endif // GLFRAMEWORK_H