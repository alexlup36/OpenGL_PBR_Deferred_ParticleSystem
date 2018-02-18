#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Common.h"
#include <string>

enum class TextureType
{
	Diffuse1,
	Diffuse2,
	Normal1,
	Normal2,
	Specular,
	Displacement,
	Depth,

	// PBR texture types
	Roughness,
	AmbientOcclusion,
	Metalness,

	Count,
};

class Texture2D
{
public:
	Texture2D(GLuint textureHandler, TextureType textureType);
	Texture2D(const std::string& sFileName, TextureType type, bool bImmutableStorage = true, bool bEnableMipmaps = true);
	virtual ~Texture2D() {}

	void bind(GLuint program);
	GLuint getHandler();
	inline const std::string& getPath() const { return m_sTexturePath; }
	inline const TextureType getTextureType() const { return m_textureType; }

protected:

private:
	Texture2D( const Texture2D& other ) {}
	void operator=( const Texture2D& other ) {}

	TextureType m_textureType;
	GLuint m_uiTexture;
	bool m_bSRGB;
	std::string m_sTexturePath;
};

#endif // __TEXTURE_H__