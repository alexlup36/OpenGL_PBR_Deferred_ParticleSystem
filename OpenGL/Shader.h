#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>

class Shader
{
public:
	Shader(const std::string& vsFilePath, const std::string& psFilePath);
	~Shader();

	const inline void useShader() { glUseProgram(m_program); }
	const inline GLuint program() const { return m_program; }

private:
	GLuint m_program;

private:
	void readShaderFromFile(const std::string& shaderFilePath, std::string& outShaderCode);
	bool compileShader(const std::string& shaderFilePath, GLuint shaderObject, const std::string& shaderCode);
	bool linkProgram();

};

#endif // SHADER_H