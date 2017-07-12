#include "Common.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <memory>

#include "GUI.h"
#include "Shader.h"

int windowWidth = 1600;
int windowHeight = 900;

std::unique_ptr<Shader> basicShader = nullptr;
std::unique_ptr<Shader> quadShader = nullptr;
GUI* gui = nullptr;

// Framebuffer (render target)
GLuint colorTexture;
GLuint depthTexture;

static const struct
{
	float x, y;
	float r, g, b;
} 

vertices[3] =
{
	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
	{ 0.6f, -0.4f, 0.f, 1.f, 0.f },
	{ 0.f,  0.6f, 0.f, 0.f, 1.f }
};

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void loadShaders()
{
	basicShader = std::make_unique<Shader>(".\\Shaders\\basic.vert", ".\\Shaders\\basic.frag");
	quadShader = std::make_unique<Shader>(".\\Shaders\\quad.vert", ".\\Shaders\\quad.frag");
}

void initGUI(GLFWwindow* window, int windowWidth, int windowHeight)
{
	gui = new GUI();
	gui->setup(window, windowWidth, windowHeight);
}

void cleanup(GLFWwindow* window)
{
	if (gui != nullptr)
	{
		delete gui;
		gui = nullptr;
	}

	// Should be done last
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLuint renderTextureToScreenSetup()
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

bool createRenderTarget(GLuint* framebuffer,
	GLint internalFormat,
	GLenum elementFormat,
	GLenum elementType,
	GLsizei width,
	GLsizei height,
	bool createDepth,
	GLenum depthFormat)
{
	assert(framebuffer != nullptr);
	GLuint fb = 0;

	// Create a render target
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);

	// Create a render target color texture
	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, elementFormat, elementType, nullptr);
	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Create the render target depth texture
	if (createDepth)
	{
		GLuint depthTexture;

		//glGenRenderbuffers(1, &depthTexture);
		//glBindRenderbuffer(GL_RENDERBUFFER, depthTexture);
		//glRenderbufferStorage(GL_RENDERBUFFER, depthFormat, width, height);
		//
		//// Attach the depth texture to the render target
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTexture);

		// If we need the data from the depth buffer use a texture instead of a render buffer
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	}

	// Color attachment tracking index
	unsigned int colorAttachmentIndex = 0;

	// Attach the color texture to the render target
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, colorTexture, 0);

	// Set the list of draw buffers
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	// Check framebuffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Copy the framwbuffer to output
	*framebuffer = fb;

	// Success
	return true;
}

int main(void)
{
	GLFWwindow* window;
	GLuint vertex_buffer;
	GLint mvp_location, vpos_location, vcol_location;
	GLint renderedTexture_location;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif // DEBUG
	window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glewExperimental = true;

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return 0;
	}
	glfwSwapInterval(1);

#ifdef _DEBUG
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// Enable OpenGL debug output
		glEnable(GL_DEBUG_OUTPUT);
		// Enable synchronous output
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// Set the callback method for debugging
		glDebugMessageCallback(glDebugOutput, nullptr);
		// Option to filter the messages we get
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	else
		std::cout << "Failed to create a OpenGL debug context.\n";
#endif // DEBUG

	// Create render target
	GLuint framebuffer = 0;
	bool res = createRenderTarget(&framebuffer,
		GL_RGBA16F,
		GL_RGBA,
		GL_FLOAT,
		windowWidth,
		windowHeight,
		true,
		GL_DEPTH_COMPONENT24);
	if (res == false)
	{
		std::cout << "Failed to create framebuffer.\n";
	}

	// Load shaders
	loadShaders();
	GLuint basicShaderProgram = basicShader->program();
	GLuint quadShaderProgram = quadShader->program();

	// Get uniform and attribute locations
	mvp_location = glGetUniformLocation(basicShaderProgram, "MVP");
	vpos_location = glGetAttribLocation(basicShaderProgram, "vPos");
	vcol_location = glGetAttribLocation(basicShaderProgram, "vCol");

	renderedTexture_location = glGetUniformLocation(quadShaderProgram, "renderedTexture");

	// Generate and bind vertex array object (Required for OpenGL context > 3.1)
	GLuint vertexArrayObject = 0;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Generate and set vertex data
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	 
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));

	// Initialize GUI
	initGUI(window, windowWidth, windowHeight);

	GLuint vaoQuad = renderTextureToScreenSetup();
	
	while (!glfwWindowShouldClose(window))
	{
		// ----------------------------------------------------------
		// Render to texture
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		float ratio;
		int width, height;
		glm::mat4 m, p, mvp;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw triangle
		glBindVertexArray(vertexArrayObject);
		m = glm::mat4();
		p = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
		mvp = p * m;
		basicShader->useShader();
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ----------------------------------------------------------
		// Render color texture to screen

		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Set the viewport and clear the color and depth buffers
		glViewport(0.0f, 0.0f, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable the quad shader
		quadShader->useShader();

		// Bind the quad vao
		glBindVertexArray(vaoQuad);

		// Bind the rendered texture to texture unit
		GLuint textureUnit = 0;
		// Activate texture unit 0
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		// Bind the rendered texture to texture unit
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		// Set the sampler to use the texture unit
		glUniform1i(renderedTexture_location, textureUnit);

		// Draw the rendered texture to the back buffer
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ----------------------------------------------------------
		// Draw GUI
		gui->draw();

		// ----------------------------------------------------------

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	cleanup(window);
	
	return 0;
}