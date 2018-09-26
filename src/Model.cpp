#include "Model.h"

// ----------------------------------------------------------------------------

//template <>
//Model<VertexPTNT>::Model(const std::string& filePath)
//{
//	if (filePath.length() == 0)
//	{
//		std::cout << "Invalid filePath specified: " << filePath << "\n";
//		return;
//	}
//
//	loadModel(filePath);
//}
//
//template <>
//Model<VertexPTNT>::~Model()
//{
//}
//
//template <>
//Model<VertexPN>::Model(const std::string& filePath)
//{
//	if (filePath.length() == 0)
//	{
//		std::cout << "Invalid filePath specified: " << filePath << "\n";
//		return;
//	}
//
//	loadModel(filePath);
//}
//
//template <>
//Model<VertexPN>::~Model()
//{
//}

// ----------------------------------------------------------------------------

//template <>
//void Model<VertexPTNT>::render()
//{
//	for each (auto mesh in m_meshList)
//		mesh.render();
//}
//
//template <>
//void Model<VertexPN>::render()
//{
//	for each (auto mesh in m_meshList)
//		mesh.render();
//}

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

template <>
Mesh<VertexP> Model<VertexP>::processMesh(aiMesh* pModel)
{
	std::vector<VertexP> vertexList;

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
		vertexList.push_back(vertex);
	}

	return Mesh<VertexP>(vertexList, processIndices(pModel));
}

// ----------------------------------------------------------------------------

template <>
Mesh<VertexPN> Model<VertexPN>::processMesh(aiMesh* pModel)
{
	std::vector<VertexPN> vertexList;

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
		vertexList.push_back(vertex);
	}

	return Mesh<VertexPN>(vertexList, processIndices(pModel));
}

// ----------------------------------------------------------------------------

template <>
Mesh<VertexPC> Model<VertexPC>::processMesh(aiMesh* pModel)
{
	std::vector<VertexPC> vertexList;

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
		vertexList.push_back(vertex);
	}

	return Mesh<VertexPC>(vertexList, processIndices(pModel));
}

// ----------------------------------------------------------------------------

template <>
Mesh<VertexPTNT> Model<VertexPTNT>::processMesh(aiMesh* pModel)
{
	std::vector<VertexPTNT> vertexList;

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
		vertexList.push_back(vertex);
	}

	return Mesh<VertexPTNT>(vertexList, processIndices(pModel));
}

// ----------------------------------------------------------------------------

template <>
Mesh<VertexPTT> Model<VertexPTT>::processMesh(aiMesh* pModel)
{
	std::vector<VertexPTT> vertexList;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < pModel->mNumVertices; i++)
	{
		// Vertex structure to be filled -------------------------------------------
		VertexPTT vertex;

		// Position ----------------------------------------------------------------
		glm::vec3 position, tangent;
		glm::vec2 textureCoodinate;

		position.x = pModel->mVertices[i].x;
		position.y = pModel->mVertices[i].y;
		position.z = pModel->mVertices[i].z;

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
		}
		else
		{
			tangent = glm::vec3();
		}

		// Init vertex data
		vertex.position = position;
		vertex.textureCoord = textureCoodinate;
		vertex.tangent = tangent;

		// Add vertex to vertex list
		vertexList.push_back(vertex);
	}

	return Mesh<VertexPTT>(vertexList, processIndices(pModel));
}

// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------