#ifndef GLFRAMEWORK_H
#define GLFRAMEWORK_H

#include "OpenGLApp.h"
#include "Framebuffer.h"
#include <memory>
#include "Model.h"
#include "TextureMan.h"
#include "Object.h"

class GUI;
class Camera;
class Shader;

class GLFramework : public OpenGLApp
{
public:
	GLFramework(int windowWidth, int windowHeight, const CameraMan& cameraMan);
	~GLFramework();

	virtual void update(double dt);
	virtual void draw(double dt);

	virtual bool initialize(const char* windowTitle, bool enableMultisampling, bool enableSRGB);

private:
	virtual bool setupScene();
	virtual void drawScene(double dt);
	void drawToGBuffer(double dt);
	void drawDeferredLighting(double dt);
	void drawForwardLighting(double dt);
	void drawGbufferToScreen();
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

	// Diplay buffer size
	GLsizei m_displayWidth;
	GLsizei m_displayHeight;

	Framebuffer m_displayFramebuffer;
	Framebuffer m_shadowFramebuffer;
	Framebuffer m_gbufferFramebuffer;

	// CameraMan reference
	const CameraMan& m_cameraMan;

	// Shaders
	Shader m_basicShader, m_finalShader, m_phongColorShader, m_phongTextureShader, m_parallaxMapping, m_normalMapping;
	Shader m_colorPBR, m_pbr, m_depth, m_skyBox, m_gbuffer, m_quadShader, m_deferredLighting;

	// Textures
	std::unique_ptr<Texture2D> m_depthMap = nullptr;

	Texture2D* m_brick1Diffuse = nullptr;
	Texture2D* m_brick1Displacement = nullptr;
	Texture2D* m_brick1Normal = nullptr;
	Texture2D* m_brick1Specular = nullptr;

	// Fabric1 pbr
	Texture2D* m_fabric1Diffuse = nullptr;
	Texture2D* m_fabric1Displacement = nullptr;
	Texture2D* m_fabric1AmbientOcclusion = nullptr;
	Texture2D* m_fabric1Normal = nullptr;

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
	std::unique_ptr<Model<VertexPTNT>> m_chair;

	// Objects
	std::unique_ptr<Object<VertexPTNT>> m_planeObject;
	std::unique_ptr<Object<VertexPN>> m_pointLightObject;

	std::unique_ptr<Object<VertexPNTT>> m_planeObjectDeferred;
	std::unique_ptr<Object<VertexPNTT>> m_torusModelDeferred;
};

#endif // GLFRAMEWORK_H