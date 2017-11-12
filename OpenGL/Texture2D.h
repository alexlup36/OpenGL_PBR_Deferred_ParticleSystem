#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Common.h"
#include <string>

enum class TextureType
{
	Diffuse,
	Normal,
	Specular,
	Displacement,
};

enum class PBRTextureType
{
	Albedo,
	Roughness,
	AmbientOcclusion,
	Metalness,
};

class Texture2D
{
public:
	Texture2D( GLuint textureHandler );
	Texture2D(const std::string& sFileName, TextureType type, bool bImmutableStorage = true, bool bEnableMipmaps = true);
	Texture2D(const std::string& sFileName, PBRTextureType type, bool bImmutableStorage = true, bool bEnableMipmaps = true);
	virtual ~Texture2D();

	void Bind(unsigned int unit, GLuint program);
	GLuint GetHandler();
	inline const std::string& getPath() const { return m_sTexturePath; }

protected:

private:
	Texture2D( const Texture2D& other ) {}
	void operator=( const Texture2D& other ) {}

	GLuint m_uiTexture;
	bool m_bSRGB;
	std::string m_sTexturePath;
};

#endif // __TEXTURE_H__