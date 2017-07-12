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
GUI* gui = nullptr;

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

int main(void)
{
	GLFWwindow* window;
	GLuint vertex_buffer;
	GLint mvp_location, vpos_location, vcol_location;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

	// Load shaders
	loadShaders();
	GLuint basicShaderProgram = basicShader->program();

	// Generate and bind vertex array object (Required for OpenGL context > 3.1)
	GLuint vertexArrayObject = 0;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Get uniform and attribute locations
	mvp_location = glGetUniformLocation(basicShaderProgram, "MVP");
	vpos_location = glGetAttribLocation(basicShaderProgram, "vPos");
	vcol_location = glGetAttribLocation(basicShaderProgram, "vCol");

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
	
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glm::mat4 m, p, mvp;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vertexArrayObject);
		m = glm::mat4();
		p = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
		mvp = p * m;
		basicShader->useShader();
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw GUI
		gui->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	cleanup(window);
	
	return 0;
}