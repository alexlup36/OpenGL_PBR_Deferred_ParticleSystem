#include "Mesh.h"

#include <iostream>

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
		m_vertexList.push_back(vertex);
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
		m_vertexList.push_back(vertex);
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
		m_vertexList.push_back(vertex);
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
		m_vertexList.push_back(vertex);
	}

	// Process indices -------------------------------------------------------------
	processIndices(pModel);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexP>::setupVertexInput(GLuint shaderProgram, const VertexP* vertexData, size_t vertexCount, const GLushort* indexData, size_t indexCount)
{
	m_vertexCount = vertexCount;

	// Generate the buffers -------------------------------------------------------------------------

	// Generate the vertex array
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// Generate the buffers and bind them
	GLuint vertexInputBuffer[1];
	glGenBuffers(1, vertexInputBuffer);
	glGenBuffers(1, &m_indexBuffer);
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexPosition)] = vertexInputBuffer[0];

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexPosition)]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Initialize the data in the buffers ------------------------------------------------------------

	// Set vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexP) * vertexCount, vertexData, GL_STATIC_DRAW);
	// Set index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indexCount, indexData, GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	int positionLocation = glGetAttribLocation(shaderProgram, "pos");
	assert(positionLocation != -1 && "Shader program not compatible with vertex format.");
	
	glEnableVertexAttribArray(static_cast<GLuint>(positionLocation)); // Enable vertex position

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), 0); // Position
																								   
	// -----------------------------------------------------------------------------------------------
 
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPN>::setupVertexInput(GLuint shaderProgram, const VertexPN* vertexData, size_t vertexCount, const GLushort* indexData, size_t indexCount)
{
	m_vertexCount = vertexCount;

	// Generate the buffers -------------------------------------------------------------------------

	// Generate the vertex array
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// Generate the buffers and bind them
	GLuint vertexInputBuffer[2];
	glGenBuffers(2, vertexInputBuffer);
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexPosition)] = vertexInputBuffer[0];
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexNormal)] = vertexInputBuffer[1];
	glGenBuffers(1, &m_indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexPosition)]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexNormal)]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Initialize the data in the buffers ------------------------------------------------------------

	// Vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vertexCount, vertexData, GL_STATIC_DRAW);
	// Indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indexCount, indexData, GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	int positionLocation = glGetAttribLocation(shaderProgram, "pos");
	int normalLocation = glGetAttribLocation(shaderProgram, "norm");
	assert(positionLocation != -1 && "Shader program not compatible with vertex format. Position location not found.");
	assert(normalLocation != -1 && "Shader program not compatible with vertex format. Normal location not found.");

	glEnableVertexAttribArray(positionLocation); // Enable vertex position
	glEnableVertexAttribArray(normalLocation); // Enable vertex normal

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), 0); // Position
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid*)(sizeof(float) * 3)); // Normal

	// -----------------------------------------------------------------------------------------------
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPC>::setupVertexInput(GLuint shaderProgram, const VertexPC* vertexData, size_t vertexCount, const GLushort* indexData, size_t indexCount)
{
	m_vertexCount = vertexCount;

	// Generate the buffers -------------------------------------------------------------------------

	// Generate the vertex array
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// Generate the buffers and bind them
	GLuint vertexInputBuffer[2];
	glGenBuffers(2, vertexInputBuffer);
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexPosition)] = vertexInputBuffer[0];
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexColor)] = vertexInputBuffer[1];
	glGenBuffers(1, &m_indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexPosition)]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexColor)]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Initialize the data in the buffers ------------------------------------------------------------

	// Vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPC) * vertexCount, vertexData, GL_STATIC_DRAW);
	// Indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indexCount, indexData, GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	int positionLocation = glGetAttribLocation(shaderProgram, "pos");
	int colorLocation = glGetAttribLocation(shaderProgram, "col");
	assert(positionLocation != -1 && "Shader program not compatible with vertex format. Position location not found.");
	assert(colorLocation != -1 && "Shader program not compatible with vertex format. Color location not found.");

	glEnableVertexAttribArray(positionLocation); // Enable vertex position
	glEnableVertexAttribArray(colorLocation); // Enable vertex color

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), 0); // Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid*)(sizeof(float) * 3)); // Color

	// -----------------------------------------------------------------------------------------------
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPTNT>::setupVertexInput(GLuint shaderProgram, const VertexPTNT* vertexData, size_t vertexCount, const GLushort* indexData, size_t indexCount)
{
	m_vertexCount = vertexCount;

	// Generate the buffers -------------------------------------------------------------------------

	// Generate the vertex array
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// Generate the buffers and bind them
	GLuint vertexInputBuffer[4];
	glGenBuffers(4, vertexInputBuffer);
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexPosition)] = vertexInputBuffer[0];
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexTextureCoord)] = vertexInputBuffer[1];
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexNormal)] = vertexInputBuffer[2];
	m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexTangent)] = vertexInputBuffer[3];
	glGenBuffers(1, &m_indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexPosition)]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexTextureCoord)]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexNormal)]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer[static_cast<int>(VertexInputType::VertexTangent)]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Initialize the data in the buffers ------------------------------------------------------------

	// Vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPTNT) * vertexCount, vertexData, GL_STATIC_DRAW);
	// Indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indexCount, indexData, GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	glEnableVertexAttribArray(0); // Enable vertex position
	glEnableVertexAttribArray(1); // Enable vertex texture
	glEnableVertexAttribArray(2); // Enable vertex normal
	glEnableVertexAttribArray(3); // Enable vertex tangent

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), 0); // Position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), (GLvoid*)(sizeof(float) * 3)); // Texture
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), (GLvoid*)(sizeof(float) * 5)); // Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), (GLvoid*)(sizeof(float) * 8)); // Tangent

	// -----------------------------------------------------------------------------------------------
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

template<class T>
void Mesh<T>::render()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_vertexCount, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------