#include "Texture3D.h"

#include <SOIL/SOIL.h>

// ----------------------------------------------------------------------------
// Constructor/Destructor

Texture3D::Texture3D(const std::string& name, const std::vector<std::string> &facePaths)
{
	assert(facePaths.size() == 6 && "Invalid number of faces specified.");

	m_textureName = name;
	for (auto& facePath : facePaths)
		m_facePaths.push_back(facePath);
}

// ----------------------------------------------------------------------------

void Texture3D::init()
{
	glCheckError();

	// Create cube map and bind it
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);

	int faceIndex = 0;
	for (auto& facePath : m_facePaths)
	{
		auto imageData = loadImageData(facePath);
		assert(imageData);
		initOpenGLTexture(imageData, faceIndex++);
	}

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Unbind the current texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glCheckError();
}

// ----------------------------------------------------------------------------

void Texture3D::bind(GLuint program)
{
	// Set the texture unit
	int textureUnit = 0;
	glActiveTexture(GLenum(GL_TEXTURE0 + textureUnit));

	// Bind the texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);
		
	// Bind the cube map sampler
	int location = glGetUniformLocation(program, "envMap");

	glUniform1i(location, textureUnit);
}

// ----------------------------------------------------------------------------

Texture3D::~Texture3D()
{
}

// ----------------------------------------------------------------------------

unsigned char* Texture3D::loadImageData(const std::string &facePath)
{
	// Load texture data from file
	unsigned char* sImageData = SOIL_load_image(facePath.c_str(), &m_iWidth, &m_iHeight, &m_iComponents, SOIL_LOAD_AUTO);

	if (sImageData == nullptr)
	{
		std::cout << "Texture loading failed " << facePath << std::endl;
		return nullptr;
	}

	return sImageData;
}

// ----------------------------------------------------------------------------

void Texture3D::initOpenGLTexture(unsigned char *imageData, unsigned int imageIndex)
{
	// Set face texture data
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + imageIndex, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	// Delete texture data
	if (imageData != nullptr)
	{
		SOIL_free_image_data(imageData);
		imageData = nullptr;
	}
}

// ----------------------------------------------------------------------------