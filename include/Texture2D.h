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

	void init();
	void bind(GLuint program);
	inline GLuint getHandler() { return m_uiTexture; }
	inline const std::string& getPath() const { return m_sTexturePath; }
	inline const TextureType getTextureType() const { return m_textureType; }

	static void bind(GLint location, GLuint textureHandle, GLuint textureUnit);

protected:

private:
	Texture2D( const Texture2D& other ) {}
	void operator=( const Texture2D& other ) {}

	// OpenGL handle
	GLuint m_uiTexture;

	TextureType m_textureType;
	std::string m_sTexturePath;

	// Texture properties
	int m_iWidth = 0, m_iHeight = 0, m_iComponents = 0;

	// Configuration
	bool m_bSRGB = false;
	bool m_immutableStorage = false;
	bool m_enableMipMaps = true;

	// Helper methods
	unsigned char* loadImageData();
	void initOpenGLTexture(unsigned char *imageData);
};

#endif // __TEXTURE_H__