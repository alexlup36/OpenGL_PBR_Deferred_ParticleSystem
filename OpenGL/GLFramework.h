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
	virtual void drawScene();
	virtual void drawSceneToDepth();

	// User interface manager
	GUI* m_pGUI;

	// VAO
	GLuint m_cubeVAO;
	GLuint m_quadVAO;
	GLuint m_skyboxVAO;

	// Depth map size
	GLsizei m_depthMapWidth = 1024;
	GLsizei m_depthMapHeight = 1024;

	// Shaders
	std::unique_ptr<Shader> m_basicShader = nullptr;
	std::unique_ptr<Shader> m_quadShader = nullptr;
	std::unique_ptr<Shader> m_phongColorShader = nullptr;
	std::unique_ptr<Shader> m_phongTextureShader = nullptr;
	std::unique_ptr<Shader> m_parallaxMapping = nullptr;
	std::unique_ptr<Shader> m_normalMapping = nullptr;
	std::unique_ptr<Shader> m_colorPBR = nullptr;
	std::unique_ptr<Shader> m_pbr = nullptr;
	std::unique_ptr<Shader> m_depth = nullptr;
	std::unique_ptr<Shader> m_skyBox = nullptr;

	// Textures
	std::unique_ptr<Texture2D> m_depthMap = nullptr;

	Texture2D* m_brick1Diffuse = nullptr;
	Texture2D* m_brick1Displacement = nullptr;
	Texture2D* m_brick1Normal = nullptr;
	Texture2D* m_brick1Specular = nullptr;

	Texture2D* m_toyBoxDiffuse = nullptr;
	Texture2D* m_toyBoxNormal = nullptr;
	Texture2D* m_toyBoxDisplacement = nullptr;

	// PBR textures
	Texture2D* m_brick1RoughnessPBR = nullptr;
	Texture2D* m_brick1MetallnessPBR = nullptr;
	Texture2D* m_brick1AmbientOcclusionPBR = nullptr;
	// Rusted iron
	Texture2D* m_rustedIronAlbedo = nullptr;
	Texture2D* m_rustedIronRoughness = nullptr;
	Texture2D* m_rustedIronMetallic = nullptr;
	Texture2D* m_rustedIronAmbientOcclusion = nullptr;
	Texture2D* m_rustedIronNormal = nullptr;
	// Gold
	Texture2D* m_goldAlbedo = nullptr;
	Texture2D* m_goldRoughness = nullptr;
	Texture2D* m_goldMetallic = nullptr;
	Texture2D* m_goldAmbientOcclusion = nullptr;
	Texture2D* m_goldNormal = nullptr;
	
	// Cube maps
	Texture3D* m_cubeMap1 = nullptr;
	Texture3D* m_cubeMap2 = nullptr;

	// FPS camera
	std::unique_ptr<Camera> m_pCamera1;
	// Render target
	std::unique_ptr<RenderTarget> m_pRT;
	std::unique_ptr<RenderTarget> m_pDT;

	std::unique_ptr<Model<VertexPN>> m_pTorusModel;
	std::unique_ptr<Model<VertexPN>> m_pMonkeyModel;
	std::unique_ptr<Model<VertexPTNT>> m_pPlaneModel;
	std::unique_ptr<Model<VertexPN>> m_pLightModel;
	std::unique_ptr<Model<VertexPTNT>> m_pSphereModel;
	std::unique_ptr<Model<VertexPTNT>> m_bunny;
	std::unique_ptr<Model<VertexPTNT>> m_dragon;
	std::unique_ptr<Model<VertexPTNT>> m_lucy;
	std::unique_ptr<Model<VertexPTNT>> m_armadillo;
	std::unique_ptr<Model<VertexPTNT>> m_tyra;
	std::unique_ptr<Model<VertexPTNT>> m_buddha;
};

#endif // GLFRAMEWORK_H