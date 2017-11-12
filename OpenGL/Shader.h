#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "Common.h"

#include "glm\glm.hpp"
#include "glm\vec2.hpp"
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"
#include "glm\mat4x4.hpp"

enum class ShaderUniform
{
	ModelMat,
	NormalMat,
	ViewMat,
	ProjMat,
	LightDir,
	LightColor,
	ObjectColor,
	ViewPos,
	Shininess,
	RenderedTexture,
	SpecularStrength,

	DiffuseTexture,
	NormalTexture,
	SpecularTexture,
	DisplacementTexture,

	Count,
};

class Shader
{
public:
	Shader(const std::string& vsFilePath, const std::string& psFilePath);
	~Shader();

	const inline void useShader() { glUseProgram(m_program); }
	const inline GLuint program() const { return m_program; }

	template<typename T>
	void set(ShaderUniform uniform, T val);

private:
	GLuint m_program;

	GLint m_shaderUniforms[static_cast<int>(ShaderUniform::Count)];

private:
	void readShaderFromFile(const std::string& shaderFilePath, std::string& outShaderCode);
	bool compileShader(const std::string& shaderFilePath, GLuint shaderObject, const std::string& shaderCode);
	bool linkProgram();

};

#endif // SHADER_H

// ----------------------------------------------------------------------------

// Template specialization
template <> inline void Shader::set<glm::mat4&>(ShaderUniform uniform, glm::mat4& val) { glUniformMatrix4fv(m_shaderUniforms[static_cast<int>(uniform)], 1, GL_FALSE, (const GLfloat*)&val[0][0]); }
template <> inline void Shader::set<glm::vec2&>(ShaderUniform uniform, glm::vec2& val) { glUniform2f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y); }
template <> inline void Shader::set<glm::vec3&>(ShaderUniform uniform, glm::vec3& val) { glUniform3f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y, val.z); }
template <> inline void Shader::set<glm::vec4&>(ShaderUniform uniform, glm::vec4& val) { glUniform4f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y, val.z, val.w); }
template <> inline void Shader::set<float>(ShaderUniform uniform, float val) { glUniform1f(m_shaderUniforms[static_cast<int>(uniform)], val); }
template <> inline void Shader::set<int>(ShaderUniform uniform, int textureUnit) { glUniform1i(m_shaderUniforms[static_cast<int>(uniform)], textureUnit); }

// ----------------------------------------------------------------------------
