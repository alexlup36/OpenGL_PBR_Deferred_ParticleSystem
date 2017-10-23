#include "Mesh.h"

#include <iostream>

// ----------------------------------------------------------------------------

template <class T>
Mesh<T>::Mesh()
{
}

template <>
Mesh<VertexPTNT>::Mesh()
{

}

// ----------------------------------------------------------------------------

template <class T>
Mesh<T>::~Mesh()
{
}

template <>
Mesh<VertexPTNT>::~Mesh()
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

template <>
void Mesh<VertexPTNT>::loadMesh(const std::string& path)
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

template <>
void Mesh<VertexP>::processMesh(aiMesh* pModel)
{
	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < pModel->mNumVertices; i++)
	{
		// Vertex structure to be filled -------------------------------------------
		VertexP vertex;

		// Position ----------------------------------------------------------------
		glm::vec3 position;
		position.x = pModel->mVertices[i].x;
		position.y = pModel->mVertices[i].y;
		position.z = pModel->mVertices[i].z;

		// Init vertex data
		vertex.position = position;

		// Add vertex to vertex list
		m_vVertexList.push_back(vertex);
	}

	// Process indices -------------------------------------------------------------
	processIndices(pModel);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPN>::processMesh(aiMesh* pModel)
{
	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < pModel->mNumVertices; i++)
	{
		// Vertex structure to be filled -------------------------------------------
		VertexPN vertex;

		// Position ----------------------------------------------------------------
		glm::vec3 position, normal;
		position.x = pModel->mVertices[i].x;
		position.y = pModel->mVertices[i].y;
		position.z = pModel->mVertices[i].z;

		// Normal ------------------------------------------------------------------
		if (pModel->HasNormals())
		{
			normal.x = pModel->mNormals[i].x;
			normal.y = pModel->mNormals[i].y;
			normal.z = pModel->mNormals[i].z;
		}

		// Init vertex data
		vertex.position = position;
		vertex.normal = normal;

		// Add vertex to vertex list
		m_vVertexList.push_back(vertex);
	}

	// Process indices -------------------------------------------------------------
	processIndices(pModel);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPC>::processMesh(aiMesh* pModel)
{
	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < pModel->mNumVertices; i++)
	{
		// Vertex structure to be filled -------------------------------------------
		VertexPC vertex;

		// Position ----------------------------------------------------------------
		glm::vec3 position;
		glm::vec4 color;
		position.x = pModel->mVertices[i].x;
		position.y = pModel->mVertices[i].y;
		position.z = pModel->mVertices[i].z;

		// Colors ------------------------------------------------------------------
		if (pModel->HasVertexColors(0))
		{
			color.x = pModel->mColors[0][i].r;
			color.y = pModel->mColors[0][i].g;
			color.z = pModel->mColors[0][i].b;
			color.w = pModel->mColors[0][i].a;
		}

		// Init vertex data
		vertex.position = position;
		vertex.color = color;

		// Add vertex to vertex list
		m_vVertexList.push_back(vertex);
	}

	// Process indices -------------------------------------------------------------
	processIndices(pModel);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPTNT>::processMesh(aiMesh* pModel)
{
	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < pModel->mNumVertices; i++)
	{
		// Vertex structure to be filled -------------------------------------------
		VertexPTNT vertex;

		// Position ----------------------------------------------------------------
		glm::vec3 position, normal, tangent, bitangent;
		glm::vec2 textureCoodinate;

		position.x = pModel->mVertices[i].x;
		position.y = pModel->mVertices[i].y;
		position.z = pModel->mVertices[i].z;

		// Normal ------------------------------------------------------------------
		if (pModel->HasNormals())
		{
			normal.x = pModel->mNormals[i].x;
			normal.y = pModel->mNormals[i].y;
			normal.z = pModel->mNormals[i].z;
		}
		else
			normal = glm::vec3();

		// Texture coordinates -----------------------------------------------------
		if (pModel->HasTextureCoords(0))
		{
			textureCoodinate.x = pModel->mTextureCoords[0][i].x;
			textureCoodinate.y = pModel->mTextureCoords[0][i].y;
		}
		else
			textureCoodinate = glm::vec2();

		// Tangent and bitangent ---------------------------------------------------
		if (pModel->HasTangentsAndBitangents())
		{
			tangent.x = pModel->mTangents[i].x;
			tangent.y = pModel->mTangents[i].y;
			tangent.z = pModel->mTangents[i].z;

			bitangent.x = pModel->mBitangents[i].x;
			bitangent.y = pModel->mBitangents[i].y;
			bitangent.z = pModel->mBitangents[i].z;
		}
		else
		{
			tangent = glm::vec3();
			bitangent = glm::vec3();
		}

		// Init vertex data
		vertex.position = position;
		vertex.normal = normal;
		vertex.textureCoord = textureCoodinate;
		vertex.tangent = tangent;
		vertex.bitangent = bitangent;

		// Add vertex to vertex list
		m_vVertexList.push_back(vertex);
	}

	// Process indices -------------------------------------------------------------
	processIndices(pModel);
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
			m_vIndexList.push_back(face.mIndices[j]);
		}
	}
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