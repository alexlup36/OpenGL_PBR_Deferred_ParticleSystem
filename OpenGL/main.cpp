#include "Common.h"

#include <iostream>

#include "GLFramework.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <memory>

#include "Shader.h"

#include "Mesh.h"
#include "Camera.h"
#include "Input.h"

std::unique_ptr<Shader> basicShader = nullptr;
std::unique_ptr<Shader> quadShader = nullptr;
std::unique_ptr<Shader> phongShader = nullptr;
std::unique_ptr<Camera> camera = nullptr;

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
	phongShader = std::make_unique<Shader>(".\\Shaders\\phong.vert", ".\\Shaders\\phong.frag");
}

GLuint renderCubeSetup()
{
	// Create cube vertex array
	GLuint cubeVertexArray;
	glGenVertexArrays(1, &cubeVertexArray);
	glBindVertexArray(cubeVertexArray);

	// Cube vertex data
	static const GLfloat cubeVertexData[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// Create cube vertex buffer
	GLuint cubeVertexBuffer;
	glGenBuffers(1, &cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(cubeVertexData),
		static_cast<const void*>(cubeVertexData),
		GL_STATIC_DRAW);

	// Vertex buffer bind
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);

	// Vertex attribute
	// Postion
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);

	// Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));

	// Unbind vertex array
	glBindVertexArray(0);

	return cubeVertexArray;
}

GLuint renderQuadSetup()
{
	// Create quad vertex object
	GLuint quadVertexArray;
	glGenVertexArrays(1, &quadVertexArray);
	glBindVertexArray(quadVertexArray);

	// Quad vertex data
	static const GLfloat quadVertexData[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	};
	// Create quad vertex buffer
	GLuint quadVertexBuffer;
	glGenBuffers(1, &quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData),
		static_cast<const void*>(quadVertexData),
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);

	// Vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));

	// Unbind vertex array object
	glBindVertexArray(0);

	return quadVertexArray;
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
	int windowWidth = 1600;
	int windowHeight = 900;

	// Create GLFramework instance
	std::unique_ptr<GLFramework> glFramework = std::make_unique<GLFramework>(windowWidth, windowHeight);
	// Initialize GLFramework
	bool enableMultisampling = true;
	bool enableSRGBFbSupport = true;
	if (glFramework->initialize("GLFramework", enableMultisampling, enableSRGBFbSupport) == false)
		return -1;
	
	// Main loop
	do
	{
		static double previousTime = glfwGetTime();
		double currentTime = glfwGetTime();
		double dt = currentTime - previousTime;

		glFramework->update(dt);
		glFramework->draw(dt);

		previousTime = currentTime;
	} 
	while (!glfwWindowShouldClose(glFramework->window()));

	GLFWwindow* window;
	double deltaTime = 0.0f;
	double lastFrame = 0.0f;

	GLuint vertex_buffer;

	// Basic
	GLint mvp_location, vpos_location, vcol_location;

	// Quad shader
	GLint renderedTexture_location;

	// Phong
	GLint modelMat_location, normalMat_location, viewMat_location, projMat_location;
	GLint lightDir_location, lightColor_location, objectColor_location, viewPos_location, shininess_location;

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

	glEnable(GL_DEPTH_TEST);

	// Cull face
	
	// Enable face culling
	//glEnable(GL_CULL_FACE);
	// Cull back faces
	glCullFace(GL_BACK);
	// Set the winding order for a face to be considered front facing
	glFrontFace(GL_CCW);

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
	GLuint phongShaderProgram = phongShader->program();

	// Load meshes
	Mesh<VertexPTNT> mesh1;
	mesh1.loadMesh("..//Assets//nanosuit.obj");

	// Get uniform and attribute locations

	// Basic shader
	mvp_location = glGetUniformLocation(basicShaderProgram, "MVP");
	vpos_location = glGetAttribLocation(basicShaderProgram, "vPos");
	vcol_location = glGetAttribLocation(basicShaderProgram, "vCol");

	// Quad shader
	renderedTexture_location = glGetUniformLocation(quadShaderProgram, "renderedTexture");

	// Phong shader
	modelMat_location = glGetUniformLocation(phongShaderProgram, "model");
	normalMat_location = glGetUniformLocation(phongShaderProgram, "normalMat");
	viewMat_location = glGetUniformLocation(phongShaderProgram, "view");
	projMat_location = glGetUniformLocation(phongShaderProgram, "projection");
	lightDir_location = glGetUniformLocation(phongShaderProgram, "lightDir");
	lightColor_location = glGetUniformLocation(phongShaderProgram, "lightColor");
	objectColor_location = glGetUniformLocation(phongShaderProgram, "objectColor");
	viewPos_location = glGetUniformLocation(phongShaderProgram, "viewPos");
	shininess_location = glGetUniformLocation(phongShaderProgram, "shininess");

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

	GLuint vaoQuadRenderToTexture = renderTextureToScreenSetup();
	GLuint vaoCube = renderCubeSetup();
	GLuint vaoQuad = renderQuadSetup();
	
	while (!glfwWindowShouldClose(window))
	{
		// ----------------------------------------------------------
		// Frame time
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;

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
		/*glBindVertexArray(vertexArrayObject);
		m = glm::mat4();
		p = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
		mvp = p * m;
		basicShader->useShader();
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/

		// ------------------------------------------------------
		// Draw cube
		glBindVertexArray(vaoCube);
		float scaleFactor = 0.1f;
		glm::vec3 position = glm::vec3(0.0f, 0.0f, -2.0f);
		m = glm::mat4();
		m = glm::scale(m, glm::vec3(scaleFactor));
		m = glm::translate(m, position);
		glm::quat quat = glm::quat(gui->m_rotation[3], gui->m_rotation[0], gui->m_rotation[1], gui->m_rotation[2]);
		glm::mat4 rotMat = glm::toMat4(quat);
		m = m * rotMat;
		p = glm::perspective(glm::radians(60.0f), static_cast<float>(windowWidth) / windowHeight, 0.1f, 100.0f);
		glm::mat4 v = glm::mat4(1.0f);// camera->viewMatrix();

		phongShader->useShader();

		// Matrices
		glUniformMatrix4fv(modelMat_location, 1, GL_FALSE, (const GLfloat*)&m[0][0]);
		glm::mat4 normalMat = glm::transpose(glm::inverse(m));
		glUniformMatrix4fv(normalMat_location, 1, GL_FALSE, (const GLfloat*)&normalMat[0][0]);
		glUniformMatrix4fv(viewMat_location, 1, GL_FALSE, (const GLfloat*)&v[0][0]);
		glUniformMatrix4fv(projMat_location, 1, GL_FALSE, (const GLfloat*)&p[0][0]);

		// Light color
		glUniform3f(lightColor_location, gui->m_lightColor[0], gui->m_lightColor[1], gui->m_lightColor[2]);
		// Light direction
		glUniform3f(lightDir_location, -gui->m_lightDirection[0], -gui->m_lightDirection[1], -gui->m_lightDirection[2]);
		// Object color
		glUniform4f(objectColor_location, gui->m_objectColor[0], gui->m_objectColor[1], gui->m_objectColor[2], gui->m_objectColor[3]);
		// Set view position
		glUniform3f(viewPos_location, 0.0f, 0.0f, 0.0f);
		// Set specular shininess
		glUniform1f(shininess_location, gui->m_shininess);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ------------------------------------------------------
		// Draw quad
		glBindVertexArray(vaoQuad);
		position = glm::vec3(0.0f, -1.0f, -2.0f);
		scaleFactor = 100.0f;
		m = glm::mat4();
		m = glm::translate(m, position);
		m = glm::rotate(m, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//m = m * rotMat;
		m = glm::scale(m, glm::vec3(scaleFactor, 1.0f, scaleFactor));

		phongShader->useShader();

		glUniformMatrix4fv(modelMat_location, 1, GL_FALSE, (const GLfloat*)&m[0][0]);
		normalMat = glm::transpose(glm::inverse(m));
		glUniformMatrix4fv(normalMat_location, 1, GL_FALSE, (const GLfloat*)&normalMat[0][0]);
		glUniformMatrix4fv(viewMat_location, 1, GL_FALSE, (const GLfloat*)&v[0][0]);
		glUniformMatrix4fv(projMat_location, 1, GL_FALSE, (const GLfloat*)&p[0][0]);

		// Light color
		glUniform3f(lightColor_location, gui->m_lightColor[0], gui->m_lightColor[1], gui->m_lightColor[2]);
		// Light direction
		glUniform3f(lightDir_location, -gui->m_lightDirection[0], -gui->m_lightDirection[1], -gui->m_lightDirection[2]);
		// Object color
		glm::vec4 quadColor = glm::vec4(0.2f, 0.6f, 0.9f, 1.0f);
		glUniform4f(objectColor_location, quadColor.x, quadColor.y, quadColor.z, quadColor.w);
		// Set view position
		glUniform3f(viewPos_location, 0.0f, 0.0f, 0.0f);
		// Set specular shininess
		glUniform1f(shininess_location, gui->m_shininess);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ----------------------------------------------------------
		// Render color texture to screen

		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Set the viewport and clear the color and depth buffers
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable the quad shader
		quadShader->useShader();

		// Bind the quad vao
		glBindVertexArray(vaoQuadRenderToTexture);

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

		glfwSwapBuffers(window);
		glfwPollEvents();

		lastFrame = currentFrame;
	}
	
	return 0;
}