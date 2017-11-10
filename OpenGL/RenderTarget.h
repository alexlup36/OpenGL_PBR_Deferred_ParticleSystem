#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "Common.h"

class RenderTarget
{
public:
	RenderTarget(GLint internalFormat,
		GLenum elementFormat,
		GLenum elementType,
		GLsizei width,
		GLsizei heightbool,
		bool createDepth = false, 
		GLenum depthFormat = GL_DEPTH_COMPONENT24);
	~RenderTarget();

	bool initialize();

	void renderToTexture();
	void renderToScreen(int x, int y, int width, int height, GLuint textureUnit);

	// Getters/setters
	inline const GLsizei width() const { return m_width; }
	inline const GLsizei height() const { return m_height; }

	static const GLuint screenQuadVAO() { return m_uiScreenQuadVAO; }

private:

	bool m_depthEnabled;

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