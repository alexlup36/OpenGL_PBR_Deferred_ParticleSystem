#include "TextureMan.h"

#include <assert.h>

// ----------------------------------------------------------------------------

TextureMan::TextureMan()
{
}

// ----------------------------------------------------------------------------

TextureMan::~TextureMan()
{
	for (auto currentTexture : m_vTextureList)
	{
		if (currentTexture != nullptr)
		{
			delete currentTexture;
			currentTexture = nullptr;
		}
	}
}

// ----------------------------------------------------------------------------

Texture3D* TextureMan::getTexture(const std::string& texturePath, const std::vector<std::string> &faceNames)
{
	// Look for the texture
	auto texture = m_textureCubeMap.find(texturePath);
	if (texture != m_textureCubeMap.end()) return texture->second;
	else
	{
		// Create texture
		return createTexture3D(texturePath, faceNames);
	}
}

// ----------------------------------------------------------------------------

Texture2D* TextureMan::getTexture(const std::string& texturePath, TextureType textureType)
{
	// Look for the texture
	auto texture = m_textureMap.find(texturePath);
	if (texture != m_textureMap.end())
	{
		// Texture found
		if (texture->second->getTextureType() == textureType) 
			return texture->second;
		else 
			return createTexture2D(texturePath, textureType);
	}
	else
		return createTexture2D(texturePath, textureType);
}

// ----------------------------------------------------------------------------

Texture2D* TextureMan::createTexture2D(const std::string& texturePath, TextureType textureType)
{
	auto newTexture = new Texture2D(texturePath, textureType);
	if (newTexture != nullptr)
	{
		newTexture->init();
		m_textureMap[texturePath] = newTexture;
	}
	return newTexture;
}

// ----------------------------------------------------------------------------

Texture3D* TextureMan::createTexture3D(const std::string& texturePath, const std::vector<std::string> &faceNames)
{
	auto newTexture = new Texture3D(texturePath, faceNames);
	if (newTexture != nullptr)
	{
		newTexture->init();
		m_textureCubeMap[texturePath] = newTexture;
	}
	return newTexture;
}

// ----------------------------------------------------------------------------