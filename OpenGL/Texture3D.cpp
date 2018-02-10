#include "Texture3D.h"

#include <SOIL2\SOIL2.h>

// ----------------------------------------------------------------------------
// Constructor/Destructor

Texture3D::Texture3D(const std::string& name, const std::vector<std::string>& facePaths)
{
	m_textureName = name;
	m_facePaths.resize(facePaths.size());

	// Create texture and load faces
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTexture);

	// Store face file paths
	int index = 0;
	for (auto& facePath : facePaths)
	{
		// Store face path
		m_facePaths[index] = facePaths[index];

		// Load face texture data
		int uiWidth, uiHeight, uiComponents;
		unsigned char* sImageData = SOIL_load_image(facePath.c_str(), &uiWidth, &uiHeight, &uiComponents, 4);

		if (sImageData != nullptr)
			std::cout << "Texture data loaded successfully " << facePath << std::endl;
		else
			std::cout << "Texture loading failed " << facePath << std::endl;

		// Set face texture data
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB, uiWidth, uiHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sImageData);
		// Set texture parameters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Delete texture data
		if (sImageData != nullptr)
		{
			SOIL_free_image_data(sImageData);
			sImageData = nullptr;
		}

		++index;
	}

	// Unbind the current texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
}

Texture3D::~Texture3D()
{
}

// ----------------------------------------------------------------------------