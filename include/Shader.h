#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "Common.h"
#include "LightData.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

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
	DebugVisualisationLightColor,
	DebugVisualisationLightDirection,
	DebugVisualisationObjectColor,

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

	Enabled,

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

	Enabled,

	Count,
};

enum class SpotLightUniform
{
	Position,
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

	Enabled,

	Count,
};

enum class MaterialUniform
{
	AmbientComp,
	DiffuseComp,
	SpecularComp,
	Shineness,

	Enabled,

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

	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,

		COUNT,
	};

	Shader();
	~Shader();

	bool initialize();
	const inline void useShader() { glUseProgram(m_program); }
	const inline GLuint program() const { return m_program; }
	bool addShader(ShaderType type, const std::string &path);

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
	void setDirLight(DirLightUniform uniform, unsigned int index, const T& val);
	template<typename T>
	void setDirLightScalar(DirLightUniform uniform, unsigned int index, float val);
	template<typename T>
	void setPointLight(PointLightUniform uniform, unsigned int index, const T& val);
	template<typename T>
	void setPointLightScalar(PointLightUniform uniform, unsigned int index, float val);
	template<typename T>
	void setSpotLight(SpotLightUniform uniform, unsigned int index, const T& val);
	template<typename T>
	void setSpotLightScalar(SpotLightUniform uniform, unsigned int index, T val);

	// Set lighting
	void updatePointLights();
	void updateDirectionalLights();
	void updateSpotLights();
	void updateDebugLight();

private:
	GLuint m_program;
	std::vector<int> m_shaderObjects;

	GLint m_shaderUniforms[static_cast<int>(ShaderUniform::Count)];
	GLint m_pointLightsUniforms[MAX_POINT_LIGHTS][static_cast<int>(PointLightUniform::Count)];
	GLint m_dirLightsUniforms[MAX_DIR_LIGHTS][static_cast<int>(DirLightUniform::Count)];
	GLint m_spotLightsUniforms[MAX_SPOT_LIGHTS][static_cast<int>(SpotLightUniform::Count)];
	GLint m_materialUniforms[static_cast<int>(MaterialUniform::Count)];
	GLint m_materialPBRUniforms[static_cast<int>(MaterialPBRUniform::Count)];

private:
	bool readShaderFromFile(const std::string& shaderFilePath, std::string& outShaderCode);
	bool compileShader(GLuint shaderObject, const std::string& shaderCode);
	bool linkProgram();
	void initializeUniforms();

};

#endif // SHADER_H

// ----------------------------------------------------------------------------

// Template specialization - generic shader uniforms
template <> inline void Shader::set<glm::mat4>(ShaderUniform uniform, const glm::mat4& val) { glUniformMatrix4fv(m_shaderUniforms[static_cast<int>(uniform)], 1, GL_FALSE, (const GLfloat*)&val[0][0]); }
template <> inline void Shader::set<glm::vec2>(ShaderUniform uniform, const glm::vec2& val) { glUniform2f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y); }
template <> inline void Shader::set<glm::vec3>(ShaderUniform uniform, const glm::vec3& val) { glUniform3f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y, val.z); }
template <> inline void Shader::set<glm::vec4>(ShaderUniform uniform, const glm::vec4& val) { glUniform4f(m_shaderUniforms[static_cast<int>(uniform)], val.x, val.y, val.z, val.w); }
template <> inline void Shader::setScalar<float>(ShaderUniform uniform, float val) { glUniform1f(m_shaderUniforms[static_cast<int>(uniform)], val); }
template <> inline void Shader::setScalar<unsigned int>(ShaderUniform uniform, unsigned int textureUnit) { glUniform1i(m_shaderUniforms[static_cast<int>(uniform)], textureUnit); }

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
inline void Shader::setDirLightScalar<float>(DirLightUniform uniform, unsigned int index, float val)
{
	assert(index < MAX_DIR_LIGHTS && "Invalid dir light index specified.");
	glUniform1f(m_dirLightsUniforms[index][static_cast<int>(uniform)], val);
}
template<>
inline void Shader::setDirLight<glm::vec3>(DirLightUniform uniform, unsigned int index, const glm::vec3& val)
{
	assert(index < MAX_DIR_LIGHTS && "Invalid dir light index specified.");
	glUniform3f(m_dirLightsUniforms[index][static_cast<int>(uniform)], val.x, val.y, val.z);
}
// Point light
template<>
inline void Shader::setPointLightScalar<float>(PointLightUniform uniform, unsigned int index, float val)
{
	assert(index < MAX_POINT_LIGHTS && "Invalid point light index specified.");
	glUniform1f(m_pointLightsUniforms[index][static_cast<int>(uniform)], val);
}
template<>
inline void Shader::setPointLight<glm::vec3>(PointLightUniform uniform, unsigned int index, const glm::vec3& val)
{
	assert(index < MAX_POINT_LIGHTS && "Invalid point light index specified.");
	glUniform3f(m_pointLightsUniforms[index][static_cast<int>(uniform)], val.x, val.y, val.z);

}
// Spot light
template<>
inline void Shader::setSpotLightScalar<float>(SpotLightUniform uniform, unsigned int index, float val)
{
	assert(index < MAX_SPOT_LIGHTS && "Invalid spot light index specified.");
	glUniform1f(m_spotLightsUniforms[index][static_cast<int>(uniform)], val);
}
template<>
inline void Shader::setSpotLight<glm::vec3>(SpotLightUniform uniform, unsigned int index, const glm::vec3& val)
{
	assert(index < MAX_SPOT_LIGHTS && "Invalid spot light index specified.");
	glUniform3f(m_spotLightsUniforms[index][static_cast<int>(uniform)], val.x, val.y, val.z);
}

// ----------------------------------------------------------------------------