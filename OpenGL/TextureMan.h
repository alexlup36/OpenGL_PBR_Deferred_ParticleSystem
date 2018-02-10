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

	template<typename T>
	void addTexture(T* texture);
	Texture2D* getTexture(const std::string& textureName, TextureType textureType);

private:
	std::vector<Texture2D*> m_vTextureList;
	std::map<const std::string, Texture2D*> m_textureMap;
};

// ----------------------------------------------------------------------------

#endif // TEXTUREMAN_H