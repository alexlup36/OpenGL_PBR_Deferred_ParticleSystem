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
	: m_sTexturePath(sFileName), m_textureType(type), m_immutableStorage(bImmutableStorage), m_enableMipMaps(bEnableMipmaps)
{
	// --------------------------------------------------------------------------
	// Load texture data from file

	// Get the texture data from the file

	// Using soil
	/*unsigned char* sImageData = SOIL_load_image( sFileName.c_str(), &m_iWidth, &m_iHeight, &m_iComponents, 4);

	if ( sImageData == nullptr )
	{
		std::cout << "Texture loading failed " << sFileName << std::endl;
	}
	else
	{
		std::cout << "Texture data loaded successfully " << sFileName << std::endl;
	}*/

	// --------------------------------------------------------------------------

	//// Create the texture handle
	//glGenTextures( 1, &m_uiTexture );
	//// Bind the current texture to the texture2D target
	//glBindTexture( GL_TEXTURE_2D, m_uiTexture );

	//// --------------------------------------------------------------------------

	//// Set texture parameters
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//// --------------------------------------------------------------------------

	//if (type == TextureType::Diffuse1 || type == TextureType::Diffuse2)
	//	m_bSRGB = true;
	//switch (type)
	//{
	//case TextureType::Diffuse1:
	//case TextureType::Diffuse2:
	//	m_bSRGB = true;
	//	break;
	//case TextureType::Normal1:
	//case TextureType::Normal2:
	//	m_bSRGB = false;
	//	break;
	//case TextureType::Specular:
	//	m_bSRGB = false;
	//	break;
	//case TextureType::Displacement:
	//	m_bSRGB = false;
	//case TextureType::AmbientOcclusion:
	//	m_bSRGB = false;
	//	break;
	//case TextureType::Metalness:
	//	m_bSRGB = false;
	//	break;
	//case TextureType::Roughness:
	//	m_bSRGB = false;
	//default:
	//	break;
	//}

	//// Allocate memory for the texture
	//if (m_bSRGB)
	//{
	//	// Mutable storage - Set texture data - convert colors to linear space so we can apply color correction
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sImageData);
	//}
	//else
	//{
	//	if (bImmutableStorage)
	//	{
	//		// Immutable storage allocation
	//		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_iWidth, m_iHeight);
	//		// Copy texture data
	//		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, sImageData);
	//	}
	//	else
	//	{
	//		// Mutable storage - Set texture data
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sImageData);
	//	}
	//}

	//// --------------------------------------------------------------------------

	//// Enable mip-maps
	//if ( bEnableMipmaps == true )
	//{
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}

	//// --------------------------------------------------------------------------

	//// Delete texture data
	//if (sImageData != nullptr)
	//{
	//	SOIL_free_image_data(sImageData);
	//	sImageData = nullptr;
	//}

	//// --------------------------------------------------------------------------
	//	
	//// Unbind the current texture
	//glBindTexture( GL_TEXTURE_2D, 0 );
}

// ----------------------------------------------------------------------------

void Texture2D::init()
{
	auto imageData = loadImageData();
	assert(imageData);
	initOpenGLTexture(imageData);
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
	glBindTexture( GL_TEXTURE_2D, m_uiTexture );

#define SETUNIFORM(locationName, textureChoise) \
	case textureChoise: \
	{ \
		int location = glGetUniformLocation(program, locationName); \
		glUniform1i(location, textureType); \
		break; \
	}

	switch (m_textureType)
	{
		SETUNIFORM("diffuseTexture1", TextureType::Diffuse1);
		SETUNIFORM("diffuseTexture2", TextureType::Diffuse2);
		SETUNIFORM("normalTexture1", TextureType::Normal1);
		SETUNIFORM("normalTexture2", TextureType::Normal2);
		SETUNIFORM("displacementTexture", TextureType::Displacement);
		SETUNIFORM("specularTexture", TextureType::Specular);
		SETUNIFORM("roughnessTexture", TextureType::Roughness);
		SETUNIFORM("metalnessTexture", TextureType::Metalness);
		SETUNIFORM("aoTexture", TextureType::AmbientOcclusion);
		SETUNIFORM("depthTexture", TextureType::Depth);

	//case TextureType::Diffuse1:
	//{
	//	// Bind the diffuse sampler
	//	int location = glGetUniformLocation(program, "diffuseTexture1");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::Diffuse2:
	//{
	//	// Bind the diffuse sampler2
	//	int location = glGetUniformLocation(program, "diffuseTexture2");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::Normal1:
	//{
	//	// Bind the normal map sampler
	//	int location = glGetUniformLocation(program, "normalTexture1");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::Normal2:
	//{
	//	// Bind the normal map sampler2
	//	int location = glGetUniformLocation(program, "normalTexture2");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::Displacement:
	//{
	//	// Bind the displacement map sampler
	//	int location = glGetUniformLocation(program, "displacementTexture");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::Specular:
	//{
	//	// Bind the specular map sampler
	//	int location = glGetUniformLocation(program, "specularTexture");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::Roughness:
	//{
	//	// Bind the roughness map sampler
	//	int location = glGetUniformLocation(program, "roughnessTexture");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::Metalness:
	//{
	//	// Bind the metalness map sampler
	//	int location = glGetUniformLocation(program, "metalnessTexture");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::AmbientOcclusion:
	//{
	//	// Bind the specular map sampler
	//	int location = glGetUniformLocation(program, "aoTexture");

	//	glUniform1i(location, textureType);
	//	break;
	//}
	//case TextureType::Depth:
	//{
	//	// Bind the depth/shadow map sampler
	//	int location = glGetUniformLocation(program, "depthTexture");

	//	glUniform1i(location, textureType);
	//	break;
	//}
			
	default:
		break;
	}
}

// ----------------------------------------------------------------------------

void Texture2D::bind(GLint location, GLuint textureHandle, GLuint textureUnit)
{
	assert(location != -1 && "Invalid uniform location.");

	glActiveTexture(GLenum(GL_TEXTURE0 + textureUnit));
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(location, textureUnit);
}

// ----------------------------------------------------------------------------


unsigned char* Texture2D::loadImageData()
{
	// Load texture data from file
	unsigned char* sImageData = SOIL_load_image(m_sTexturePath.c_str(), &m_iWidth, &m_iHeight, &m_iComponents, 4);

	if (sImageData == nullptr)
	{
		std::cout << "Texture loading failed " << m_sTexturePath << std::endl;
		return nullptr;
	}

	return sImageData;
}

// ----------------------------------------------------------------------------

void Texture2D::initOpenGLTexture(unsigned char *imageData)
{
	glCheckError();

	// Create the texture handle
	glGenTextures(1, &m_uiTexture);
	// Bind the current texture to the texture2D target
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);

	// Set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Use SRGB textures for diffuse types
	if (m_textureType == TextureType::Diffuse1 || m_textureType == TextureType::Diffuse2)
		m_bSRGB = true;

	// Allocate memory for the texture
	if (m_bSRGB)
	{
		// Mutable storage - Set texture data - convert colors to linear space so we can apply color correction
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}
	else
	{
		if (m_immutableStorage)
		{
			// Immutable storage allocation
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_iWidth, m_iHeight);
			// Copy texture data
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		}
		else
		{
			// Mutable storage - Set texture data
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		}
	}

	// --------------------------------------------------------------------------

	if (m_enableMipMaps == true) glGenerateMipmap(GL_TEXTURE_2D);

	// --------------------------------------------------------------------------

	// Delete texture data
	if (imageData != nullptr)
	{
		SOIL_free_image_data(imageData);
		imageData = nullptr;
	}

	// --------------------------------------------------------------------------

	// Unbind the current texture
	glBindTexture(GL_TEXTURE_2D, 0);

	glCheckError();
}

// ----------------------------------------------------------------------------