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
	Mesh(const std::string& filePath, GLuint shaderProgram);
	//Mesh(const T* vertexData, size_t vertexCount, const GLushort* indexData, size_t indexCount);
	Mesh(GLuint vertexArrayObject)
		: m_uiVertexArrayObject(vertexArrayObject) {}
	~Mesh();

	inline void loadMesh(const std::string& path);
	inline const GLuint vertexArrayObject() const { return m_vertexArrayObject; }
	void render();

	static GLuint vaoCubeSetup();
	static GLuint vaoQuadSetup();

private:

	enum class VertexInputType
	{
		VertexPosition,
		VertexTextureCoord,
		VertexNormal,
		VertexTangent,
		VertexColor,

		NumOfBuffers,
	};

	void setupVertexInput(GLuint shaderProgram, const T* vertexData, size_t vertexCount, const GLushort* indexData, size_t indexCount);
	void processMesh(aiMesh* pModel);
	
	// Template specialization
	void recursiveProcess(aiNode* pNode, const aiScene* pScene);
	void processIndices(aiMesh* pModel);

	GLuint m_vertexArrayObject;
	GLuint m_indexBuffer;
	size_t m_vertexCount;
	GLuint m_vertexArrayBuffer[VertexInputType::NumOfBuffers];

	std::vector<T> m_vertexList;
	std::vector<GLushort> m_indexList;
};

// ----------------------------------------------------------------------------

template <class T>
Mesh<T>::Mesh(const std::string& filePath, GLuint shaderProgram)
{
	assert(filePath.length() != 0 && "Invalid file path specified.");
	loadMesh(filePath);
	setupVertexInput(shaderProgram, m_vertexList.data(), m_vertexCount, m_indexList.data(), m_indexList.size());
}

template <class T>
Mesh<T>::~Mesh()
{
}

// ----------------------------------------------------------------------------

template <class T>
void Mesh<T>::loadMesh(const std::string& path)
{
	Assimp::Importer assimpImporter;

	const aiScene* pScene = assimpImporter.ReadFile(path.c_str(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_ImproveCacheLocality);

	if (pScene == nullptr)
	{
		std::cout << "Mesh load failed!: " << path << " " << assimpImporter.GetErrorString() << std::endl;
		return;
	}
	else if (pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || pScene->mRootNode == false)
	{
		std::cout << "Failed to load file " << path << " " << assimpImporter.GetErrorString() << std::endl;
		return;
	}
	else
	{
		std::cout << "Mesh loaded successfully: " << path << std::endl;
	}

	recursiveProcess(pScene->mRootNode, pScene);
}

// ----------------------------------------------------------------------------

template <class T>
void Mesh<T>::recursiveProcess(aiNode* pNode, const aiScene* pScene)
{
	// Process
	for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
	{
		aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
		processMesh(mesh);
	}

	// Recursion
	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		recursiveProcess(pNode->mChildren[i], pScene);
	}
}

// ----------------------------------------------------------------------------

template <class T>
void Mesh<T>::processIndices(aiMesh* pModel)
{
	for (unsigned int i = 0; i < pModel->mNumFaces; i++)
	{
		aiFace face = pModel->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m_indexList.push_back(face.mIndices[j]);
		}
	}
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
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));

	// Unbind vertex array object
	glBindVertexArray(0);

	return quadVertexArray;
}

// ----------------------------------------------------------------------------

#endif // MESH_H