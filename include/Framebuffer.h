#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Common.h"
#include <map>
#include <vector>
#include <assert.h>

class Framebuffer
{
public:

	enum class RenderTargetType
	{
		COLOR_TARGET,
		DEPTH_TARGET
	};

	Framebuffer() = default;
	~Framebuffer() = default;

	Framebuffer &initialize(GLsizei width, GLsizei height);
	bool create();
	Framebuffer &addColorTarget(const std::string &rtName, GLint internalFormat, GLenum elementFormat, GLenum elementType);
	Framebuffer &addDepthTarget(GLint internalFormat, GLsizei width = 0, GLsizei height = 0, bool readDepth = true);

	void renderToTexture(RenderTargetType targetType);
	void renderColorTargetToScreen(int x, int y, int width, int height, GLuint textureUnit);
	void renderDepthTargetToScreen(int x, int y, int width, int height, GLuint textureUnit);
	inline void set() const { glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferHandle); }
	inline void unset() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	// Getters/setters
	inline const GLsizei width() const { return m_width; }
	inline const GLsizei height() const { return m_height; }

	// Get handles to color and depth buffer
	inline const GLuint colorTexture(int index) const { assert(index < m_colorTextures.size() && "Invalid color texture index specified."); return m_colorTextures[index]; }
	inline const GLuint colorTexture(const std::string &rtName) const 
	{
		auto element = m_colorTextureNames.find(rtName);
		if (element != m_colorTextureNames.end())
		{
			return m_colorTextures[element->second];
		}
	}
	inline const GLuint depthTexture() const { return m_depthTexture; }

	static const GLuint screenQuadVAO() { return m_uiScreenQuadVAO; }

private:

	bool m_depthEnabled = false;
	bool m_readDepth = false;

	GLuint m_depthTexture;
	
	GLbitfield m_clearMask = 0;
	GLsizei m_width;
	GLsizei m_height;
	GLuint m_framebufferHandle;

	std::map<std::string, GLuint> m_colorTextureNames;
	std::vector<GLuint> m_colorTextures;

	static GLuint m_uiScreenQuadVAO;
	static GLuint renderTextureToScreenSetup();
};

#endif // FRAMEBUFFER_H