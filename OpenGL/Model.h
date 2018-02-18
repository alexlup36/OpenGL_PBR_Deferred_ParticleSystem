#ifndef MODEL_H
#define MODEL_H

// ----------------------------------------------------------------------------

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

// ----------------------------------------------------------------------------

template<class T>
class Model
{
public:
	Model(const std::string& filePath);
	~Model();

	void render();

private:
	
	void loadModel(const std::string& filePath);

	Mesh<T> processMesh(aiMesh* pModel);
	void processNode(aiNode* pNode, const aiScene* pScene);
	std::vector<GLushort> processIndices(aiMesh* pModel);

	std::vector<Mesh<T>> m_meshList;
};

// ----------------------------------------------------------------------------

template <class T>
Model<T>::Model(const std::string& filePath)
{
	if (filePath.length() == 0)
	{
		std::cout << "Invalid filePath specified: " << filePath << "\n";
		return;
	}

	loadModel(filePath);
}

// ----------------------------------------------------------------------------

template <class T>
Model<T>::~Model()
{
}

// ----------------------------------------------------------------------------

template <class T>
void Model<T>::render()
{
	for each (auto mesh in m_meshList)
		mesh.render();
}

// ----------------------------------------------------------------------------

template <class T>
void Model<T>::loadModel(const std::string& filePath)
{
	Assimp::Importer assimpImporter;

	const aiScene* pScene = assimpImporter.ReadFile(filePath.c_str(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_ImproveCacheLocality);

	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || pScene->mRootNode == false)
	{
		std::cout << "Mesh load failed!: " << filePath << " " << assimpImporter.GetErrorString() << std::endl;
		return;
	}
	else
		std::cout << "Mesh loaded successfully: " << filePath << std::endl;

	processNode(pScene->mRootNode, pScene);
}

// ----------------------------------------------------------------------------

template <class T>
void Model<T>::processNode(aiNode* pNode, const aiScene* pScene)
{
	// Process
	for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
	{
		aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
		m_meshList.push_back(processMesh(mesh));
	}

	// Recursion
	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		processNode(pNode->mChildren[i], pScene);
	}
}

// ----------------------------------------------------------------------------

template <class T>
std::vector<GLushort> Model<T>::processIndices(aiMesh* pModel)
{
	std::vector<GLushort> indexList;

	for (unsigned int i = 0; i < pModel->mNumFaces; i++)
	{
		aiFace& face = pModel->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indexList.push_back(face.mIndices[j]);
	}

	return indexList;
}

// ----------------------------------------------------------------------------

#endif // MODEL_H