#ifndef TEXTUREMAN_H
#define TEXTUREMAN_H

// ----------------------------------------------------------------------------

#include <vector>
#include <map>

#include "Texture2D.h"
#include "Texture3D.h"

// ----------------------------------------------------------------------------

class TextureMan
{
private:
	TextureMan(void);
	~TextureMan(void);

public:

	// Static access function
	static TextureMan& Instance()
	{
		static TextureMan refInstance;
		return refInstance;
	}

	Texture3D* getTexture(const std::string& texturePath, const std::vector<std::string> &faceNames);
	Texture2D* getTexture(const std::string& texturePath, TextureType textureType);

private:
	std::vector<Texture2D*> m_vTextureList;
	std::map<const std::string, Texture2D*> m_textureMap;
	std::map<const std::string, Texture3D*> m_textureCubeMap;

	Texture2D* createTexture2D(const std::string& texturePath, TextureType textureType);
	Texture3D* createTexture3D(const std::string& texturePath, const std::vector<std::string> &faceNames);
};

// ----------------------------------------------------------------------------

#endif // TEXTUREMAN_H