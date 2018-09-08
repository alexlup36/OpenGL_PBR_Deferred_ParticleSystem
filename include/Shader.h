#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "Common.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

enum class ShaderUniform
{
	// Matrices
	ModelMat,
	NormalMat,
	ViewMat,
	ProjMat,
	LightMat,

	LightDir,
	LightColor,
	ObjectColor,
	ViewPos,
	Shininess,
	RenderedTexture,
	SpecularStrength,

	TextureOffset,
	TextureTile,

	// Parallax mapping / normal mapping
	DisplacementMapScale,
	NormalMapScale,

	// HDR
	Gamma,

	// Textures
	DiffuseTexture,
	NormalTexture,
	SpecularTexture,
	DisplacementTexture,
	DepthTexture,

	// Tone mapper
	ToneMapper,
	Exposure,
	GammaHDR,
	ExposureBias,

	// Debug
	DisplayMode,

	Count,
};

enum class PointLightUniform
{
	Position,
	Attenuation,

	// Defined for PBR materials
	Color,

	// Defined for materials rendered using Phong
	ColorAmbientComp,
	ColorDiffuseComp,
	ColorSpecularComp,

	Count,
};

enum class DirLightUniform
{
	Direction,

	// Defined for PBR materials
	Color,

	// Defined for materials rendered using Phong
	ColorAmbientComp,
	ColorDiffuseComp,
	ColorSpecularComp,

	Count,
};

enum class SpotLightUniform
{
	Position,
	Attenuation,
	Direction,
	Exponent,
	Cutoff,
	CosCutoff,

	// Defined for PBR materials
	Color,

	// Defined for materials rendered using Phong
	ColorAmbientComp,
	ColorDiffuseComp,
	ColorSpecularComp,

	Count,
};

enum class MaterialUniform
{
	AmbientComp,
	DiffuseComp,
	SpecularComp,
	Shineness,

	Count,
};

enum class MaterialPBRUniform
{
	Color,
	Metallic,
	Roughness,
	AmbientOcclusion,

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
	void set(ShaderUniform uniform, const T& val);
	template<typename T>
	void setScalar(ShaderUniform uniform, T val);

	template<typename T>
	void setMaterial(MaterialUniform uniform, const T& val);
	template<typename T>
	void setMaterialScalar(MaterialUniform uniform, T val);
	template<typename T>
	void setMaterial(MaterialPBRUniform uniform, const T& val);
	template<typename T>
	void setMaterialScalar(MaterialPBRUniform uniform, T val);

	template<typename T>
	void setDirLight(DirLightUniform uniform, int index, const T& val);
	template<typename T>
	void setPointLight(PointLightUniform uniform, int index, const T& val);
	template<typename T>
	void setSpotLight(SpotLightUniform uniform, int index, const T& val);
	template<typename T>
	void setSpotLightScalar(SpotLightUniform uniform, int index, T val);

private:
	GLuint m_program;

	GLint m_shaderUniforms[static_cast<int>(ShaderUniform::Count)];
	GLint m_pointLightsUniforms[MAX_POINT_LIGHTS][static_cast<int>(PointLightUniform::Count)];
	GLint m_dirLightsUniforms[MAX_DIR_LIGHTS][static_cast<int>(DirLightUniform::Count)];
	GLint m_spotLightsUniforms[MAX_SPOT_LIGHTS][static_cast<int>(SpotLightUniform::Count)];
	GLint m_materialUniforms[static_cast<int>(MaterialUniform::Count)];
	GLint m_materialPBRUniforms[static_cast<int>(MaterialPBRUniform::Count)];

private:
	void readShaderFromFile(const std::string& shaderFilePath, std::string& outShaderCode);
	bool compileShader(const std::string& shaderFilePath, GLuint shaderObject, const std::string& shaderCode);
	bool linkProgram();

};

#endif // SHADER_H

// ----------------------------------------------------------------------------

// Template specialization - generic shader uniforms
template <> inline void Shader::set<glm::mat4>(ShaderUniform uniform, const glm::mat4& val) { glUniformMatrix4fv(m_shaderUniforms[static_cast<int>(uniform)], 1, GL_FALSE, (const GLfloat*)&val[0][0]); }
template <> inline void Shader::set<glm::vec2>(ShaderUniform uniform, const glm::vec2& val) { glUniform2f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y); }
template <> inline void Shader::set<glm::vec3>(ShaderUniform uniform, const glm::vec3& val) { glUniform3f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y, val.z); }
template <> inline void Shader::set<glm::vec4>(ShaderUniform uniform, const glm::vec4& val) { glUniform4f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y, val.z, val.w); }
template <> inline void Shader::setScalar<float>(ShaderUniform uniform, float val) { glUniform1f(m_shaderUniforms[static_cast<int>(uniform)], val); }
template <> inline void Shader::setScalar<int>(ShaderUniform uniform, int textureUnit) { glUniform1i(m_shaderUniforms[static_cast<int>(uniform)], textureUnit); }

// Material
template<>
inline void Shader::setMaterial<glm::vec3>(MaterialUniform uniform, const glm::vec3& val)
{
	glUniform3f(m_materialUniforms[static_cast<int>(uniform)], val.x, val.y, val.z);
}
template<>
inline void Shader::setMaterialScalar<float>(MaterialUniform uniform, float val)
{
	glUniform1f(m_materialUniforms[static_cast<int>(uniform)], val);
}
// PBR material color
template<>
inline void Shader::setMaterial<glm::vec3>(MaterialPBRUniform uniform, const glm::vec3& val)
{
	glUniform3f(m_materialPBRUniforms[static_cast<int>(uniform)], val.x, val.y, val.z);
}
template<>
inline void Shader::setMaterialScalar<float>(MaterialPBRUniform uniform, float val)
{
	glUniform1f(m_materialPBRUniforms[static_cast<int>(uniform)], val);
}

// Directional light
template<>
inline void Shader::setDirLight<glm::vec3>(DirLightUniform uniform, int index, const glm::vec3& val)
{
	assert(index < MAX_DIR_LIGHTS && "Invalid dir light index specified.");
	glUniform3f(m_dirLightsUniforms[index][static_cast<int>(uniform)], val.x, val.y, val.z);
}
// Point light
template<>
inline void Shader::setPointLight<glm::vec3>(PointLightUniform uniform, int index, const glm::vec3& val)
{
	assert(index < MAX_POINT_LIGHTS && "Invalid point light index specified.");
	glUniform3f(m_pointLightsUniforms[index][static_cast<int>(uniform)], val.x, val.y, val.z);

}
// Spot light
template<>
inline void Shader::setSpotLightScalar<float>(SpotLightUniform uniform, int index, float val)
{
	assert(index < MAX_SPOT_LIGHTS && "Invalid spot light index specified.");
	glUniform1f(m_spotLightsUniforms[index][static_cast<int>(uniform)], val);
}
template<>
inline void Shader::setSpotLight<glm::vec3>(SpotLightUniform uniform, int index, const glm::vec3& val)
{
	assert(index < MAX_SPOT_LIGHTS && "Invalid spot light index specified.");
	glUniform3f(m_spotLightsUniforms[index][static_cast<int>(uniform)], val.x, val.y, val.z);
}

// ----------------------------------------------------------------------------
