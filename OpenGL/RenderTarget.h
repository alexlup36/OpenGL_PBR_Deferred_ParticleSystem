#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "Common.h"

class RenderTarget
{
public:
	RenderTarget(bool createColor,
		GLint internalFormat,
		GLenum elementFormat,
		GLenum elementType,
		GLsizei width,
		GLsizei heightbool,
		bool createDepth = false, 
		GLenum depthFormat = GL_DEPTH_COMPONENT24);
	~RenderTarget();

	bool initialize();

	void renderToTexture();
	void renderColorToScreen(int x, int y, int width, int height, GLuint textureUnit);
	void renderDepthToScreen(int x, int y, int width, int height, GLuint textureUnit);

	// Getters/setters
	inline const GLsizei width() const { return m_width; }
	inline const GLsizei height() const { return m_height; }

	// Get handles to color and depth buffer
	inline const GLuint colorTexture() const { return m_uiColorTexture; }
	inline const GLuint depthTexture() const { return m_uiDepthTexture; }

	static const GLuint screenQuadVAO() { return m_uiScreenQuadVAO; }

private:

	GLbitfield m_clearMask = 0;

	bool m_depthEnabled;
	bool m_colorEnabled;

	GLint m_internalFormat;
	GLenum m_elementFormat;
	GLenum m_depthFormat;
	GLenum m_elementType;
	GLsizei m_width;
	GLsizei m_height;

	GLuint m_uiFramebuffer;
	GLuint m_uiColorTexture;
	GLuint m_uiDepthTexture;

	static GLuint m_uiScreenQuadVAO;
	static GLuint renderTextureToScreenSetup();
};

#endif // RENDERTARGET_H