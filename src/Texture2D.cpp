#include "Texture2D.h"

#include <iostream>
#include <cassert>
#include <SOIL/SOIL.h>

// ----------------------------------------------------------------------------

Texture2D::Texture2D(GLuint textureHandler, TextureType textureType)
{
	m_uiTexture = textureHandler;
	m_textureType = textureType;
}

// ----------------------------------------------------------------------------

Texture2D::Texture2D(const std::string& sFileName, TextureType type, bool bImmutableStorage, bool bEnableMipmaps)
{
	// Store texture path
	m_sTexturePath = sFileName;

	m_textureType = type;

	// --------------------------------------------------------------------------
	// Load texture data from file

	int uiWidth, uiHeight, uiComponents;

	// Get the texture data from the file

	// Using soil
	unsigned char* sImageData = SOIL_load_image( sFileName.c_str(), &uiWidth, &uiHeight, &uiComponents, 4);

	if ( sImageData == nullptr )
	{
		std::cout << "Texture loading failed " << sFileName << std::endl;
	}
	else
	{
		std::cout << "Texture data loaded successfully " << sFileName << std::endl;
	}

	// --------------------------------------------------------------------------

	// Create the texture handle
	glGenTextures( 1, &m_uiTexture );
	// Bind the current texture to the texture2D target
	glBindTexture( GL_TEXTURE_2D, m_uiTexture );

	// --------------------------------------------------------------------------

	// Set texture parameters
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// --------------------------------------------------------------------------

	switch (type)
	{
	case TextureType::Diffuse1:
	case TextureType::Diffuse2:
		m_bSRGB = true;
		break;
	case TextureType::Normal1:
	case TextureType::Normal2:
		m_bSRGB = false;
		break;
	case TextureType::Specular:
		m_bSRGB = false;
		break;
	case TextureType::Displacement:
		m_bSRGB = false;
	case TextureType::AmbientOcclusion:
		m_bSRGB = false;
		break;
	case TextureType::Metalness:
		m_bSRGB = false;
		break;
	case TextureType::Roughness:
		m_bSRGB = false;
	default:
		break;
	}

	// Allocate memory for the texture
	if (m_bSRGB)
	{
		// Mutable storage - Set texture data - convert colors to linear space so we can apply color correction
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, uiWidth, uiHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sImageData);
	}
	else
	{
		if (bImmutableStorage)
		{
			// Immutable storage allocation
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, uiWidth, uiHeight);
			// Copy texture data
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, uiWidth, uiHeight, GL_RGBA, GL_UNSIGNED_BYTE, sImageData);
		}
		else
		{
			// Mutable storage - Set texture data
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, uiWidth, uiHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sImageData);
		}
	}

	// --------------------------------------------------------------------------

	// Enable mip-maps
	if ( bEnableMipmaps == true )
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// --------------------------------------------------------------------------

	// Delete texture data
	if (sImageData != nullptr)
	{
		SOIL_free_image_data(sImageData);
		sImageData = nullptr;
	}

	// --------------------------------------------------------------------------
		
	// Unbind the current texture
	glBindTexture( GL_TEXTURE_2D, 0 );
}

// ----------------------------------------------------------------------------

void Texture2D::bind(GLuint program)
{
	unsigned int textureType = static_cast<int>(m_textureType);
	unsigned int textureTypeCount = static_cast<int>(TextureType::Count);
	assert( textureType >= 0 && textureType <= textureTypeCount && textureType <= 32 &&
			"Invalid texture type specified.");

	// Set the texture unit
	glActiveTexture( GLenum(GL_TEXTURE0 + textureType) );

	// Bind the texture
	glBindTexture( GL_TEXTURE_2D, m_uiTexture );

	switch (m_textureType)
	{
	case TextureType::Diffuse1:
	{
		// Bind the diffuse sampler
		int location = glGetUniformLocation(program, "diffuseTexture1");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::Diffuse2:
	{
		// Bind the diffuse sampler2
		int location = glGetUniformLocation(program, "diffuseTexture2");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::Normal1:
	{
		// Bind the normal map sampler
		int location = glGetUniformLocation(program, "normalTexture1");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::Normal2:
	{
		// Bind the normal map sampler2
		int location = glGetUniformLocation(program, "normalTexture2");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::Displacement:
	{
		// Bind the displacement map sampler
		int location = glGetUniformLocation(program, "displacementTexture");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::Specular:
	{
		// Bind the specular map sampler
		int location = glGetUniformLocation(program, "specularTexture");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::Roughness:
	{
		// Bind the roughness map sampler
		int location = glGetUniformLocation(program, "roughnessTexture");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::Metalness:
	{
		// Bind the metalness map sampler
		int location = glGetUniformLocation(program, "metalnessTexture");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::AmbientOcclusion:
	{
		// Bind the specular map sampler
		int location = glGetUniformLocation(program, "aoTexture");

		glUniform1i(location, textureType);
		break;
	}
	case TextureType::Depth:
	{
		// Bind the depth/shadow map sampler
		int location = glGetUniformLocation(program, "depthTexture");

		glUniform1i(location, textureType);
		break;
	}
			
	default:
		break;
	}
}

// ----------------------------------------------------------------------------

GLuint Texture2D::getHandler()
{
	return m_uiTexture;
}

// ----------------------------------------------------------------------------