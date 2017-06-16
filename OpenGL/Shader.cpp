#include "Shader.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>

// ----------------------------------------------------------------------------

Shader::Shader(const std::string& vsFilePath, const std::string& psFilePath)
{
	// Create shader program
	m_program = glCreateProgram();

	// Read shader code
	std::string vsShaderCode, psShaderCode;
	readShaderFromFile(vsFilePath, vsShaderCode);
	readShaderFromFile(psFilePath, psShaderCode);

	// Create shader objects
	GLuint vsObject = glCreateShader(GL_VERTEX_SHADER);
	GLuint psObject = glCreateShader(GL_FRAGMENT_SHADER);

	// Compile shaders
	if (compileShader(vsFilePath, vsObject, vsShaderCode))
		glAttachShader(m_program, vsObject);
	if (compileShader(psFilePath, psObject, psShaderCode))
		glAttachShader(m_program, psObject);

	// Link program
	linkProgram();

	// Delete shader objects
	glDeleteShader(vsObject);
	glDeleteShader(psObject);
}

// ----------------------------------------------------------------------------

Shader::~Shader()
{
}

// ----------------------------------------------------------------------------

void Shader::readShaderFromFile(const std::string& shaderFilePath, std::string& outShaderCode)
{
	assert(shaderFilePath.length() && "Error. Empty shader path.");

	// Init shader code
	outShaderCode = "";
	// Open shader file and read content into outShaderCode
	std::ifstream shaderStream(shaderFilePath, std::ios::in);
	if (shaderStream.is_open())
	{
		std::string line = "";
		while (getline(shaderStream, line))
			outShaderCode += (line + "\n");
		shaderStream.close();
	}
	else
		std::cout << "Failed to open shader file: " << shaderFilePath << "\n";
}

// ----------------------------------------------------------------------------

bool Shader::compileShader(const std::string& shaderFilePath, GLuint shaderObject, const std::string& shaderCode)
{
	std::cout << "Compiling: " << shaderFilePath;

	// Set shader source and compile
	const char* shaderSource = shaderCode.c_str();
	glShaderSource(shaderObject, 1, &shaderSource, nullptr);
	glCompileShader(shaderObject);

	// Error check
	GLint compileStatus;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		// Error
		GLchar message[1024];
		glGetShaderInfoLog(shaderObject, 1024, nullptr, message);
		std::cout << "Shader compile failed: " << message;
		return false;
	}
	// Success
	std::cout << " success. \n";
	return true;
}

// ----------------------------------------------------------------------------

bool Shader::linkProgram()
{
	// Link program
	glLinkProgram(m_program);

	// Error check
	GLint linkStatus;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		// Error
		GLchar message[1024];
		glGetProgramInfoLog(m_program, 1024, nullptr, message);
		std::cout << "Shader link failed: " << message << "\n";
		return false;
	}
	// Success
	return true;
}

// ----------------------------------------------------------------------------