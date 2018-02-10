#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include <vector>
#include <string>
#include <cassert>

#include "Common.h"

enum class Face { Top, Bottom, Left, Right, Front, Back, Count };

class Texture3D
{
public:
	Texture3D(const std::string& name, const std::vector<std::string>& facePaths);
	~Texture3D();

	inline GLuint getHandler() const { return m_uiTexture; }
	inline const std::string& getName() const { return m_textureName; }
	inline const std::string& getFacePath(Face face) const
	{
		assert(static_cast<int>(face) < static_cast<int>(Face::Count) && "Invalid face specified.");
		return m_facePaths[static_cast<int>(face)];
	}

private:
	GLuint m_uiTexture;
	std::vector<std::string> m_facePaths;
	std::string m_textureName;
};

#endif // TEXTURE3D_H