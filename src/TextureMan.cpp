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

template<typename Texture2D>
void TextureMan::addTexture(Texture2D* texture)
{
	// If the texture doesn't exist in the map, add it
	const std::string& texturePath = texture->getPath();
	if (m_textureMap.find(texturePath) == m_textureMap.end())
	{
		m_textureMap[texturePath] = texture;
		m_vTextureList.push_back(texture);
	}
}

// ----------------------------------------------------------------------------

Texture3D* TextureMan::getTexture(const std::string& textureName, const std::vector<std::string> &faceNames)
{
	// Look for the texture
	auto texture = m_textureCubeMap.find(textureName);
	if (texture != m_textureCubeMap.end())
	{
		// We found the texture
		return texture->second;
	}
	else
	{
		// Create texture
		Texture3D* newTexture = new Texture3D(textureName, faceNames);
		if (newTexture != nullptr)
		{
			// Add texture to cache
			m_textureCubeMap[textureName] = newTexture;
			return newTexture;
		}
		else
		{
			std::cout << "Failed to create texture: " << textureName << "\n";
			assert(newTexture);
			return nullptr;
		}
	}
}

Texture2D* TextureMan::getTexture(const std::string& texturePath, 
	TextureType textureType)
{
	// Look for the texture
	auto texture = m_textureMap.find(texturePath);
	if (texture != m_textureMap.end())
	{
		if (texture->second->getTextureType() == textureType)
			// We found the texture
			return texture->second;
		else
		{
			// Create texture
			Texture2D* newTexture = new Texture2D(texturePath, textureType);
			if (newTexture != nullptr)
			{
				// Add texture to cache
				m_textureMap[texturePath] = newTexture;
				return newTexture;
			}
			else
			{
				std::cout << "Failed to create texture: " << texturePath << "\n";
				assert(newTexture);
				return nullptr;
			}
		}
	}
	else
	{
		// Create texture
		Texture2D* newTexture = new Texture2D(texturePath, textureType);
		if (newTexture != nullptr)
		{
			// Add texture to cache
			m_textureMap[texturePath] = newTexture;
			return newTexture;
		}
		else
		{
			std::cout << "Failed to create texture: " << texturePath << "\n";
			assert(newTexture);
			return nullptr;
		}
	}
}

// ----------------------------------------------------------------------------