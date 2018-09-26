#include "Framebuffer.h"

// ----------------------------------------------------------------------------

GLuint Framebuffer::m_uiScreenQuadVAO = -1;

// ----------------------------------------------------------------------------

Framebuffer &Framebuffer::initialize(GLsizei width, GLsizei height)
{
	assert(width > 0 && "Invalid width size. Specified value needs to be > 0");
	assert(height > 0 && "Invalid width size. Specified value needs to be > 0");

	// Framebuffer size
	m_width = width;
	m_height = height;

	// Create a render target
	glGenFramebuffers(1, &m_framebufferHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferHandle);

	return *this;
}

// ----------------------------------------------------------------------------

bool Framebuffer::create()
{
	std::vector<GLenum> drawBuffers;

	// Setup color buffers
	if (m_colorTextures.size() > 0)
	{
		// Enable clear color
		m_clearMask |= GL_COLOR_BUFFER_BIT;

		// Color attachment tracking index
		for (unsigned int colorAttachmentIndex = 0; colorAttachmentIndex < m_colorTextures.size(); ++colorAttachmentIndex)
		{
			// Attach the color textures to the render target
			unsigned int colorAttachment = GL_COLOR_ATTACHMENT0 + colorAttachmentIndex;
			glFramebufferTexture(GL_FRAMEBUFFER, colorAttachment, m_colorTextures[colorAttachmentIndex], 0);
			drawBuffers.push_back(colorAttachment);
		}

		// Set the list of draw buffers
		glDrawBuffers(drawBuffers.size(), drawBuffers.data());
	}
	else
	{
		// A framebuffer is not complete without a color buffer so 
		// we must specify that we are not going to render any color info
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	/// Attach the depth texture to the render target
	if (m_depthEnabled)
	{
		// Enable clear depth
		m_clearMask |= GL_DEPTH_BUFFER_BIT;

		if (m_readDepth)
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture, 0);
		else
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTexture);
	}

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

Framebuffer &Framebuffer::addColorTarget(const std::string &rtName, GLint internalFormat, GLenum elementFormat, GLenum elementType)
{
	assert(rtName != "" && "Invalid target name");

	// Generate texture handle
	GLuint colorTexture;
	glGenTextures(1, &colorTexture);
	// Bind color texture
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	// Generate color texture
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0,
		elementFormat,
		elementType,
		nullptr);
	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Add the newly created color texture to our array of color targets
	m_colorTextures.push_back(colorTexture);
	m_colorTextureNames[rtName] = colorTexture;

	return *this;
}

Framebuffer &Framebuffer::addDepthTarget(GLint internalFormat, GLsizei width, GLsizei height, bool readDepth)
{
	m_depthEnabled = true;
	m_readDepth = readDepth;

	// If we need the data from the depth buffer use a texture instead of a render buffer
	if (readDepth)
	{
		// Generate texture handle
		glGenTextures(1, &m_depthTexture);
		// Bind depth texture
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		// Generate depth texture
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width == 0 ? m_width : width, height == 0 ? m_height : height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		// Setup filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		// Generate render buffer handle
		glGenRenderbuffers(1, &m_depthTexture);
		// Bind render buffer
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthTexture);
		// Generate render buffer
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width == 0 ? m_width : width, height == 0 ? m_height : height);
		// Filtering ?
	}

	return *this;
}

// ----------------------------------------------------------------------------

GLuint Framebuffer::renderTextureToScreenSetup()
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

void Framebuffer::renderToTexture(RenderTargetType targetType)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferHandle);
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);

	if (targetType == RenderTargetType::DEPTH_TARGET)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	// Set viewport and clear color and depth
	glViewport(0, 0, m_width, m_height);
	// Clear
	glClear(m_clearMask);
}

// ----------------------------------------------------------------------------

void Framebuffer::renderColorTargetToScreen(int x, int y, int width, int height, GLuint index)
{
	assert(index < m_colorTextures.size());

	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Set the viewport and clear the color and depth buffers
	glViewport(x, y, width, height);
	// Bind the rendered texture to texture unit
	// Activate texture unit 0
	glActiveTexture(GL_TEXTURE0 + index);
	// Bind the rendered texture to texture unit
	glBindTexture(GL_TEXTURE_2D, m_colorTextures[index]);
}

void Framebuffer::renderDepthTargetToScreen(int x, int y, int width, int height, GLuint textureUnit)
{
	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Set the viewport and clear the color and depth buffers
	glViewport(x, y, width, height);
	// Bind the rendered texture to texture unit
	// Activate texture unit 0
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	// Bind the rendered texture to texture unit
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
}

// ----------------------------------------------------------------------------