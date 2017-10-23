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

// Base vertex definition containig only position data
struct VertexP
{
	VertexP()
		: position(glm::vec3()) {}
	VertexP(const glm::vec3& pos)
		: position(pos) {}

	glm::vec3 position;
};

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

template<class T>
class Mesh
{
public:
	Mesh();
	~Mesh();

	void loadMesh(const std::string& path);

private:
	void processMesh(aiMesh* pModel);
	
	// Template specialization
	void recursiveProcess(aiNode* pNode, const aiScene* pScene);
	void processIndices(aiMesh* pModel);

	std::vector<T> m_vVertexList;
	std::vector<GLushort> m_vIndexList;
};

// ----------------------------------------------------------------------------

#endif // MESH_H