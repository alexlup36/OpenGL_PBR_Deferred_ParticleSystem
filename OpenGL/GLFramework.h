#ifndef GLFRAMEWORK_H
#define GLFRAMEWORK_H

#include "OpenGLApp.h"
#include "RenderTarget.h"
#include <memory>
#include "Model.h"
#include "TextureMan.h"

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
	std::unique_ptr<Shader> m_phongColorShader = nullptr;
	std::unique_ptr<Shader> m_phongTextureShader = nullptr;
	std::unique_ptr<Shader> m_parallaxMapping = nullptr;
	std::unique_ptr<Shader> m_normalMapping = nullptr;
	std::unique_ptr<Shader> m_simplePBR = nullptr;
	std::unique_ptr<Shader> m_pbr = nullptr;

	// Textures
	Texture2D* m_brick1Diffuse = nullptr;
	Texture2D* m_brick1Displacement = nullptr;
	Texture2D* m_brick1Normal = nullptr;
	Texture2D* m_brick1Specular = nullptr;

	// FPS camera
	std::unique_ptr<Camera> m_pCamera1;
	// Render target
	std::unique_ptr<RenderTarget> m_pRT;

	std::unique_ptr<Model<VertexPN>> m_pTorusModel;
	std::unique_ptr<Model<VertexPN>> m_pMonkeyModel;
	std::unique_ptr<Model<VertexPTNT>> m_pPlaneModel;
};

#endif // GLFRAMEWORK_H