#include "Shader.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>

// ----------------------------------------------------------------------------

Shader::Shader()
{
}

// ----------------------------------------------------------------------------

Shader::~Shader()
{
}

// ----------------------------------------------------------------------------

bool Shader::initialize()
{
	assert(m_shaderObjects.size() != 0 && "Failed to initialize shader. Use add shader to set shader stages.");

	// Create shader program
	m_program = glCreateProgram();

	// Attach shader objects
	for (auto shaderObject : m_shaderObjects)
	{
		// Attach shader to shader program
		glAttachShader(m_program, shaderObject);
	}

	// Link program
	if (linkProgram() == false)
		return false;

	// Initialize uniforms
	initializeUniforms();

	// Delete shader objects
	for (auto shaderObject : m_shaderObjects)
		glDeleteShader(shaderObject);

	// Sucess
	return true;
}

// ----------------------------------------------------------------------------

bool Shader::addShader(ShaderType type, const std::string &path)
{
	// Read shader code
	std::string shaderCode;
	bool res = readShaderFromFile(path, shaderCode);
	if (res == false) return false;

	// Create shader object
	GLuint shaderObject;
	switch (type)
	{
		case ShaderType::VERTEX:
			shaderObject = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::FRAGMENT:
			shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			std::cout << "Invalid shader type.";
			break;
	}
	m_shaderObjects.push_back(shaderObject);

	// Compile shader
	if (compileShader(shaderObject, shaderCode) == false)
	{
		std::cout << "Shader " << path << " failed to compile.\n";
		return false;
	}

	// Success
	return true;
}

// ----------------------------------------------------------------------------

bool Shader::readShaderFromFile(const std::string& shaderFilePath, std::string& outShaderCode)
{
	assert(shaderFilePath.length() && "Error. Empty shader path.");

	// Init shader code
	outShaderCode = "";
	// Open shader file and read content into outShaderCode
	std::ifstream shaderStream;
	shaderStream.exceptions(std::ifstream::badbit);
  	try 
	{
    	shaderStream.open(shaderFilePath.c_str(), std::ifstream::in);

		if (shaderStream.is_open())
		{
			std::string line = "";
			while (getline(shaderStream, line))
				outShaderCode += (line + "\n");
			shaderStream.close();
		}
		else
		{
			std::cout << "Failed to open " << shaderFilePath;
			return false;
		}
  	}
  	catch (const std::ifstream::failure& e) 
	{
		std::cout << "Exception: " << e.what() << " while opening/reading file " << shaderFilePath << "\n";
		return false;
  	}

	// Success
	return true;
}

// ----------------------------------------------------------------------------

bool Shader::compileShader(GLuint shaderObject, const std::string& shaderCode)
{
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

void Shader::initializeUniforms()
{
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
	m_shaderUniforms[static_cast<int>(ShaderUniform::ToneMapper)] = glGetUniformLocation(m_program, "toneMapper");
	m_shaderUniforms[static_cast<int>(ShaderUniform::Exposure)] = glGetUniformLocation(m_program, "exposure");
	m_shaderUniforms[static_cast<int>(ShaderUniform::GammaHDR)] = glGetUniformLocation(m_program, "gammaHDR");
	m_shaderUniforms[static_cast<int>(ShaderUniform::ExposureBias)] = glGetUniformLocation(m_program, "exposureBias");

	m_shaderUniforms[static_cast<int>(ShaderUniform::DiffuseTexture)] = glGetUniformLocation(m_program, "diffuseTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::NormalTexture)] = glGetUniformLocation(m_program, "normalTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::SpecularTexture)] = glGetUniformLocation(m_program, "specularTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::DisplacementTexture)] = glGetUniformLocation(m_program, "displacementTexture");
	m_shaderUniforms[static_cast<int>(ShaderUniform::DepthTexture)] = glGetUniformLocation(m_program, "depthTexture");

	m_shaderUniforms[static_cast<int>(ShaderUniform::TextureOffset)] = glGetUniformLocation(m_program, "textureOffset");
	m_shaderUniforms[static_cast<int>(ShaderUniform::TextureTile)] = glGetUniformLocation(m_program, "textureTile");

	// Initialize dir lights uniform locations
	for (unsigned int dirLightIndex = 0; dirLightIndex < MAX_DIR_LIGHTS; ++dirLightIndex)
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
		sShaderLocation = sTemp + "enabled";
		m_dirLightsUniforms[dirLightIndex][static_cast<int>(DirLightUniform::Enabled)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
	}

	// Initialize point lights uniform locations
	for (unsigned int pointLightIndex = 0; pointLightIndex < MAX_POINT_LIGHTS; ++pointLightIndex)
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
		sShaderLocation = sTemp + "enabled";
		m_pointLightsUniforms[pointLightIndex][static_cast<int>(PointLightUniform::Enabled)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
	}
// ---------------------------------------------------------------------------// ---------------------------------------------------------------------------
	// Initialize spot lights uniform locations
	for (unsigned spotLightIndex = 0; spotLightIndex < MAX_SPOT_LIGHTS; ++spotLightIndex)
	{
		// Get the location for the current light uniform
		std::string sLightIndex = std::to_string(spotLightIndex);
		std::string sTemp = "spotLight[" + sLightIndex + "].";
		std::string sShaderLocation;

		sShaderLocation = sTemp + "position";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::Position)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
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
		sShaderLocation = sTemp + "enabled";
		m_spotLightsUniforms[spotLightIndex][static_cast<int>(SpotLightUniform::Enabled)] = glGetUniformLocation(m_program, sShaderLocation.c_str());
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
}

// ----------------------------------------------------------------------------

// Update lights
void Shader::updatePointLights()
{
	auto& lightData = LightData::getInstance();
	auto pointLightCount = lightData.pointLightCount();

	for (auto pointLightIndex = 0; pointLightIndex < pointLightCount; ++pointLightIndex)
	{
		auto &currentPointLight = lightData.pointLight(pointLightIndex);
		setPointLightScalar<float>(PointLightUniform::Enabled, pointLightIndex, currentPointLight.enabled);

		// Skip if not enabled
		if (currentPointLight.enabled == false)
			continue;
			
		// Set color
		if (currentPointLight.pbrLight)
		{
			setPointLight<glm::vec3>(PointLightUniform::Color, pointLightIndex, currentPointLight.color);
		}
		else
		{
			setPointLight<glm::vec3>(PointLightUniform::ColorAmbientComp, pointLightIndex, currentPointLight.ambientComp);
			setPointLight<glm::vec3>(PointLightUniform::ColorSpecularComp, pointLightIndex, currentPointLight.specularComp);
			setPointLight<glm::vec3>(PointLightUniform::ColorDiffuseComp, pointLightIndex, currentPointLight.diffuseComp);
		}

		// Set properties
		setPointLight<glm::vec3>(PointLightUniform::Position, pointLightIndex, currentPointLight.position);
		setPointLight<glm::vec3>(PointLightUniform::Attenuation, pointLightIndex, currentPointLight.attenuation);
	}
}

void Shader::updateDirectionalLights()
{
	auto& lightData = LightData::getInstance();
	auto dirLightCount = lightData.directionalLightCount();

	for (auto dirLightIndex = 0; dirLightIndex < dirLightCount; ++dirLightIndex)
	{
		auto &currentDirLight = lightData.directionalLight(dirLightIndex);
		setDirLightScalar<float>(DirLightUniform::Enabled, dirLightIndex, currentDirLight.enabled);

		// Skip if not enabled
		if (currentDirLight.enabled == false)
			continue;
		
		// Set color
		if (currentDirLight.pbrLight)
		{
			setDirLight<glm::vec3>(DirLightUniform::Color, dirLightIndex, currentDirLight.color);
		}
		else
		{
			setDirLight<glm::vec3>(DirLightUniform::ColorAmbientComp, dirLightIndex, currentDirLight.ambientComp);
			setDirLight<glm::vec3>(DirLightUniform::ColorSpecularComp, dirLightIndex, currentDirLight.specularComp);
			setDirLight<glm::vec3>(DirLightUniform::ColorDiffuseComp, dirLightIndex, currentDirLight.diffuseComp);
		}

		// Set properties
		setDirLight<glm::vec3>(DirLightUniform::Direction, dirLightIndex, currentDirLight.direction);
	}
}

void Shader::updateSpotLights()
{
	auto& lightData = LightData::getInstance();
	auto spotLightCount = lightData.spotLightCount();

	for (auto spotLightIndex = 0; spotLightIndex < spotLightCount; ++spotLightIndex)
	{
		auto &currentSpotLight = lightData.spotLight(spotLightIndex);
		setSpotLightScalar<float>(SpotLightUniform::Enabled, spotLightIndex, currentSpotLight.enabled);

		// Skip if not enabled
		if (currentSpotLight.enabled == false)
			continue;

		// Set color
		if (currentSpotLight.pbrLight)
		{
			setSpotLight<glm::vec3>(SpotLightUniform::Color, spotLightIndex, currentSpotLight.color);
		}
		else
		{
			setSpotLight<glm::vec3>(SpotLightUniform::ColorAmbientComp, spotLightIndex, currentSpotLight.ambientComp);
			setSpotLight<glm::vec3>(SpotLightUniform::ColorDiffuseComp, spotLightIndex, currentSpotLight.diffuseComp);
			setSpotLight<glm::vec3>(SpotLightUniform::ColorSpecularComp, spotLightIndex, currentSpotLight.specularComp);
		}
		
		// Set properties
		setSpotLight<glm::vec3>(SpotLightUniform::Position, spotLightIndex, currentSpotLight.position);
		setSpotLight<glm::vec3>(SpotLightUniform::Direction, spotLightIndex, currentSpotLight.direction);
		setSpotLightScalar<float>(SpotLightUniform::CosCutoff, spotLightIndex, currentSpotLight.coscutoff);
		setSpotLightScalar<float>(SpotLightUniform::Cutoff, spotLightIndex, currentSpotLight.cutoff);
		setSpotLightScalar<float>(SpotLightUniform::Exponent, spotLightIndex, currentSpotLight.exponent);
	}
}

// ----------------------------------------------------------------------------