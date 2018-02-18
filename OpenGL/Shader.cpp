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

	// Default values for shader uniforms
	unsigned int shaderUniformCount = static_cast<int>(ShaderUniform::Count);
	for (unsigned int index = 0; index < shaderUniformCount; ++index)
		m_shaderUniforms[index] = -1;

	// Initialize shader uniform locations
	m_shaderUniforms[static_cast<int>(ShaderUniform::ModelMat)] = glGetUniformLocation(m_program, "model");
	m_shaderUniforms[static_cast<int>(ShaderUniform::NormalMat)] = glGetUniformLocation(m_program, "normalMat");
	m_shaderUniforms[static_cast<int>(ShaderUniform::ViewMat)] = glGetUniformLocation(m_program, "view");
	m_shaderUniforms[static_cast<int>(ShaderUniform::ProjMat)] = glGetUniformLocation(m_program, "projection");
	m_shaderUniforms[static_cast<int>(ShaderUniform::LightMat)] = glGetUniformLocation(m_program, "lightVP");
	m_shaderUniforms[static_cast<int>(ShaderUniform::LightDir)] = glGetUniformLocation(m_program, "lightDir");
	m_shaderUniforms[static_cast<int>(ShaderUniform::LightColor)] = glGetUniformLocation(m_program, "lightColor");
	m_shaderUniforms[static_cast<int>(ShaderUniform::ObjectColor)] = glGetUniformLocation(m_program, "objectColor");
	m_shaderUniforms[static_cast<int>(ShaderUniform::ViewPos)] = glGetUniformLocation(m_program, "viewPos");
	m_shaderUniforms[static_cast<int>(ShaderUniform::Shininess)] = glGetUniformLocation(m_program, "shininess");
	m_shaderUniforms[static_cast<int>(ShaderUniform::SpecularStrength)] = glGetUniformLocation(m_program, "specularStrength");
	m_shaderUniforms[static_cast<int>(ShaderUniform::RenderedTexture)] = glGetUniformLocation(m_program, "renderedTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::DisplacementMapScale)] = glGetUniformLocation(m_program, "dispMapScale");
	m_shaderUniforms[static_cast<int>(ShaderUniform::NormalMapScale)] = glGetUniformLocation(m_program, "normalMapScale");
	m_shaderUniforms[static_cast<int>(ShaderUniform::Gamma)] = glGetUniformLocation(m_program, "gamma");
	m_shaderUniforms[static_cast<int>(ShaderUniform::DisplayMode)] = glGetUniformLocation(m_program, "displayMode");

	m_shaderUniforms[static_cast<int>(ShaderUniform::DiffuseTexture)] = glGetUniformLocation(m_program, "diffuseTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::NormalTexture)] = glGetUniformLocation(m_program, "normalTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::SpecularTexture)] = glGetUniformLocation(m_program, "specularTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::DisplacementTexture)] = glGetUniformLocation(m_program, "displacementTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::DepthTexture)] = glGetUniformLocation(m_program, "depthTexture");

	// Initialize dir lights uniform locations
	for (auto dirLightIndex = 0; dirLightIndex < MAX_DIR_LIGHTS; ++dirLightIndex)
	{
		// Get the location for the current light uniform
		std::string sLightIndex = std::to_string(dirLightIndex);
		std::string sTemp = "dirLight[" + sLightIndex + "].";
		std::string sShaderLocation;

		sShaderLocation = sTemp + "direction";
		m_dirLightsUniforms[dirLightIndex][static_cast<int>(DirLightUniform::Direction)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "color";
		m_dirLightsUniforms[dirLightIndex][static_cast<int>(DirLightUniform::Color)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "ambientComp";
		m_dirLightsUniforms[dirLightIndex][static_cast<int>(DirLightUniform::ColorAmbientComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "diffuseComp";
		m_dirLightsUniforms[dirLightIndex][static_cast<int>(DirLightUniform::ColorDiffuseComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "specularComp";
		m_dirLightsUniforms[dirLightIndex][static_cast<int>(DirLightUniform::ColorSpecularComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
	}

	// Initialize point lights uniform locations
	for (auto pointLightIndex = 0; pointLightIndex < MAX_POINT_LIGHTS; ++pointLightIndex)
	{
		// Get the location for the current light uniform
		std::string sLightIndex = std::to_string(pointLightIndex);
		std::string sTemp = "pointLight[" + sLightIndex + "].";
		std::string sShaderLocation;

		sShaderLocation = sTemp + "position";
		m_pointLightsUniforms[pointLightIndex][static_cast<int>(PointLightUniform::Position)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "attenuation";
		m_pointLightsUniforms[pointLightIndex][static_cast<int>(PointLightUniform::Attenuation)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "color";
		m_pointLightsUniforms[pointLightIndex][static_cast<int>(PointLightUniform::Color)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "ambientComp";
		m_pointLightsUniforms[pointLightIndex][static_cast<int>(PointLightUniform::ColorAmbientComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "diffuseComp";
		m_pointLightsUniforms[pointLightIndex][static_cast<int>(PointLightUniform::ColorDiffuseComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "specularComp";
		m_pointLightsUniforms[pointLightIndex][static_cast<int>(PointLightUniform::ColorSpecularComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
	}

	// Initialize spot lights uniform locations
	for (auto spotLightIndex = 0; spotLightIndex < MAX_SPOT_LIGHTS; ++spotLightIndex)
	{
		// Get the location for the current light uniform
		std::string sLightIndex = std::to_string(spotLightIndex);
		std::string sTemp = "spotLight[" + sLightIndex + "].";
		std::string sShaderLocation;

		sShaderLocation = sTemp + "position";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::Position)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "attenuation";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::Attenuation)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "color";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::Color)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "ambientComp";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::ColorAmbientComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "diffuseComp";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::ColorDiffuseComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "specularComp";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::ColorSpecularComp)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "dirrection";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::Direction)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "exponent";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::Exponent)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "cutoff";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::Cutoff)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
		sShaderLocation = sTemp + "cosCutoff";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::CosCutoff)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
	}

	// Initialize material uniform locations
	m_materialUniforms[static_cast<int>(MaterialUniform::AmbientComp)] = glGetUniformLocation(m_program, "material.ambientComp");
	m_materialUniforms[static_cast<int>(MaterialUniform::DiffuseComp)] = glGetUniformLocation(m_program, "material.diffuseComp");
	m_materialUniforms[static_cast<int>(MaterialUniform::SpecularComp)] = glGetUniformLocation(m_program, "material.specularComp");
	m_materialUniforms[static_cast<int>(MaterialUniform::Shineness)] = glGetUniformLocation(m_program, "material.shineness");

	// Initialize PBR color material uniform locations
	m_materialPBRUniforms[static_cast<int>(MaterialPBRUniform::Color)] = glGetUniformLocation(m_program, "material.color");
	m_materialPBRUniforms[static_cast<int>(MaterialPBRUniform::Metallic)] = glGetUniformLocation(m_program, "material.metallic");
	m_materialPBRUniforms[static_cast<int>(MaterialPBRUniform::Roughness)] = glGetUniformLocation(m_program, "material.roughness");
	m_materialPBRUniforms[static_cast<int>(MaterialPBRUniform::AmbientOcclusion)] = glGetUniformLocation(m_program, "material.ao");


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