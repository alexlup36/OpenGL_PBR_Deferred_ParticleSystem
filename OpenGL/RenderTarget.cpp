#include "RenderTarget.h"

// ----------------------------------------------------------------------------

GLuint RenderTarget::m_uiScreenQuadVAO = -1;

// ----------------------------------------------------------------------------

RenderTarget::RenderTarget(GLint internalFormat, GLenum elementFormat,
	GLenum elementType, GLsizei width, GLsizei height,
	bool createDepth, GLenum depthFormat)
	: m_internalFormat(internalFormat), m_elementFormat(elementFormat),
	m_elementType(elementType), m_width(width), m_height(height),
	m_depthEnabled(createDepth), m_depthFormat(depthFormat)
{
}

RenderTarget::~RenderTarget()
{
}

// ----------------------------------------------------------------------------

bool RenderTarget::initialize()
{
	// Create a render target
	glGenFramebuffers(1, &m_uiFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);

	// Create a render target color texture
	glGenTextures(1, &m_uiColorTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 
		0, 
		m_internalFormat, 
		m_width, 
		m_height, 
		0, 
		m_elementFormat, 
		m_elementType, 
		nullptr);
	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Create the render target depth texture
	if (m_depthEnabled)
	{
		//glGenRenderbuffers(1, &depthTexture);
		//glBindRenderbuffer(GL_RENDERBUFFER, depthTexture);
		//glRenderbufferStorage(GL_RENDERBUFFER, depthFormat, width, height);
		//
		//// Attach the depth texture to the render target
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTexture);

		// If we need the data from the depth buffer use a texture instead of a render buffer
		glGenTextures(1, &m_uiDepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_uiDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, m_depthFormat, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_uiDepthTexture, 0);
	}

	// Color attachment tracking index
	unsigned int colorAttachmentIndex = 0;

	// Attach the color texture to the render target
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, m_uiColorTexture, 0);

	// Set the list of draw buffers
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	// Check framebuffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Setup the vao for the rendering quad
	m_uiScreenQuadVAO = renderTextureToScreenSetup();

	// Success
	return true;
}

// ----------------------------------------------------------------------------

GLuint RenderTarget::renderTextureToScreenSetup()
{
	// Create quad vertex object
	GLuint quadVertexArray;
	glGenVertexArrays(1, &quadVertexArray);
	glBindVertexArray(quadVertexArray);

	// Quad vertex data
	static const GLfloat quadVertexData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	// Create quad vertex buffer
	GLuint quadVertexBuffer;
	glGenBuffers(1, &quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData),
		static_cast<const void*>(quadVertexData),
		GL_STATIC_DRAW);

	// Vertex attribute
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Unbind vertex array object
	glBindVertexArray(0);

	return quadVertexArray;
}

// ----------------------------------------------------------------------------

void RenderTarget::renderToTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);

	// Set viewport and clear color and depth
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ----------------------------------------------------------------------------

void RenderTarget::renderToScreen(int x, int y, int width, int height, GLuint textureUnit)
{
	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Set the viewport and clear the color and depth buffers
	glViewport(x, y, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the rendered texture to texture unit
	// Activate texture unit 0
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Bind the rendered texture to texture unit
	glBindTexture(GL_TEXTURE_2D, m_uiColorTexture);
}

// ----------------------------------------------------------------------------