#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Common.h"

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// Base vertex definition containig only position data
struct VertexP
{
	VertexP()
		: position(glm::vec3()) {}
	VertexP(const glm::vec3& pos)
		: position(pos) {}

	glm::vec3 position;
};

// ----------------------------------------------------------------------------

// Vertex containing position and color data
struct VertexPC : public VertexP
{
	VertexPC()
		: VertexP(), color(glm::vec4()) {}
	VertexPC(const glm::vec3& pos,
		const glm::vec4& col)
		: VertexP(pos), color(col) {}

	glm::vec4 color;
};

// ----------------------------------------------------------------------------

// Vertex containing position and normal data
struct VertexPN : public VertexP
{
	VertexPN()
		: VertexP(), normal(glm::vec3()) {}
	VertexPN(const glm::vec3& pos,
		const glm::vec3& norm)
		: VertexP(pos), normal(norm) {}

	glm::vec3 normal;
};

// ----------------------------------------------------------------------------

// Vertex containing position, texture coordinate, normal and tangent
struct VertexPTNT : public VertexPN
{
	VertexPTNT()
		: VertexPN(), textureCoord(glm::vec2()), tangent(glm::vec3()) {}
	VertexPTNT(const glm::vec3& pos,
		const glm::vec2& tex,
		const glm::vec3& norm,
		const glm::vec3& tan,
		const glm::vec3& bitan)
		:  VertexPN(pos, norm), textureCoord(tex), tangent(tan), bitangent(bitan) {}

	glm::vec2 textureCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

// ----------------------------------------------------------------------------

// Vertex containing position, normal texture coordinate and tangent
struct VertexPNTT
{
	VertexPNTT()
		: position(glm::vec3()), normal(glm::vec3()), textureCoord(glm::vec2()), tangent(glm::vec3()) {}
	VertexPNTT(const glm::vec3& pos,
		const glm::vec3& norm,
		const glm::vec2& tex,
		const glm::vec3& tan)
		:  position(pos), normal(norm), textureCoord(tex), tangent(tan) {}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoord;
	glm::vec3 tangent;
};

// ----------------------------------------------------------------------------

// Vertex containing position, texture coordinate, normal and tangent
struct VertexPTT : public VertexP
{
	VertexPTT()
		: VertexP(), textureCoord(glm::vec2()), tangent(glm::vec3()) {}
	VertexPTT(const glm::vec3& pos,
		const glm::vec2& tex,
		const glm::vec3& tan)
		:  VertexP(pos), textureCoord(tex), tangent(tan) {}

	glm::vec2 textureCoord;
	glm::vec3 tangent;
};

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

template<class T>
class Mesh
{
public:
	Mesh(std::vector<T> vertexList, std::vector<GLushort> indexList);
	~Mesh();

	inline const GLuint vertexArrayObject() const { return m_vertexArrayObject; }
	void render();

	static GLuint vaoCubeSetup();
	static GLuint vaoQuadSetup();
	static GLuint vaoSkyboxSetup();

private:

	void setupVertexInput();

	GLuint m_vertexArrayObject;

	std::vector<T> m_vertexList;
	std::vector<GLushort> m_indexList;
};

// ----------------------------------------------------------------------------

template <class T>
Mesh<T>::Mesh(std::vector<T> vertexList, std::vector<GLushort> indexList)
	: m_vertexList(vertexList), m_indexList(indexList) 
{
	setupVertexInput();
}

template <class T>
Mesh<T>::~Mesh()
{
}

// ----------------------------------------------------------------------------

template<class T>
inline GLuint Mesh<T>::vaoCubeSetup()
{
	// Create cube vertex array
	GLuint cubeVertexArray;
	glGenVertexArrays(1, &cubeVertexArray);
	glBindVertexArray(cubeVertexArray);

	// Cube vertex data
	static const GLfloat cubeVertexData[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// Create cube vertex buffer
	GLuint cubeVertexBuffer;
	glGenBuffers(1, &cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(cubeVertexData),
		static_cast<const void*>(cubeVertexData),
		GL_STATIC_DRAW);

	// Vertex buffer bind
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);

	// Vertex attribute
	// Postion
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);

	// Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));

	// Unbind vertex array
	glBindVertexArray(0);

	return cubeVertexArray;
}

// ----------------------------------------------------------------------------

template<class T>
inline GLuint Mesh<T>::vaoQuadSetup()
{
	// Create quad vertex object
	GLuint quadVertexArray;
	glGenVertexArrays(1, &quadVertexArray);
	glBindVertexArray(quadVertexArray);

	// Quad vertex data
	static const GLfloat quadVertexData[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	};
	// Create quad vertex buffer
	GLuint quadVertexBuffer;
	glGenBuffers(1, &quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData),
		static_cast<const void*>(quadVertexData),
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);

	// Vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	// Unbind vertex array object
	glBindVertexArray(0);

	return quadVertexArray;
}

// ----------------------------------------------------------------------------
template<class T>
inline GLuint Mesh<T>::vaoSkyboxSetup()
{
	// Create skybox vertex object
	GLuint skyboxVertexArray;
	glGenVertexArrays(1, &skyboxVertexArray);
	glBindVertexArray(skyboxVertexArray);

	// Quad vertex data
	static const GLfloat skyboxVertexData[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	// Create sky box vertex buffer
	GLuint skyboxVertexBuffer;
	glGenBuffers(1, &skyboxVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertexData),
		static_cast<const void*>(skyboxVertexData),
		GL_STATIC_DRAW);

	// Vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// Unbind vertex array object
	glBindVertexArray(0);

	return skyboxVertexArray;
}

// ----------------------------------------------------------------------------

template<class T>
void Mesh<T>::render()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indexList.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

#endif // MESH_H