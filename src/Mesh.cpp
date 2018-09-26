#include "Mesh.h"

#include <iostream>

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexP>::setupVertexInput()
{
	// Generate the buffers -------------------------------------------------------------------------

	GLuint vertexBuffer, indexBuffer;
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(m_vertexArrayObject);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexP) * m_vertexList.size(), (const void*)m_vertexList.data(), GL_STATIC_DRAW);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indexList.size(), (const void*)m_indexList.data(), GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	glEnableVertexAttribArray(0); // Enable vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), 0); // Position
																								   
	// -----------------------------------------------------------------------------------------------
 
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPN>::setupVertexInput()
{
	// Generate the buffers -------------------------------------------------------------------------

	GLuint vertexBuffer, indexBuffer;
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(m_vertexArrayObject);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * m_vertexList.size(), (const void*)m_vertexList.data(), GL_STATIC_DRAW);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indexList.size(), (const void*)m_indexList.data(), GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	glEnableVertexAttribArray(0); // Enable vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), 0); // Position

	glEnableVertexAttribArray(1); // Enable vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid*)(sizeof(float) * 3)); // Normal

	// -----------------------------------------------------------------------------------------------
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPC>::setupVertexInput()
{
	// Generate the buffers -------------------------------------------------------------------------

	GLuint vertexBuffer, indexBuffer;
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(m_vertexArrayObject);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPC) * m_vertexList.size(), (const void*)m_vertexList.data(), GL_STATIC_DRAW);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indexList.size(), (const void*)m_indexList.data(), GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), 0); // Position

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid*)(sizeof(float) * 3)); // Color

	// -----------------------------------------------------------------------------------------------
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPTNT>::setupVertexInput()
{
	// Generate the vertex array

	GLuint vertexBuffer, indexBuffer;
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(m_vertexArrayObject);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPTNT) * m_vertexList.size(), (const void*)m_vertexList.data(), GL_STATIC_DRAW);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indexList.size(), (const void*)m_indexList.data(), GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	glEnableVertexAttribArray(0); // Enable vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), 0); // Position

	glEnableVertexAttribArray(1); // Enable vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), (GLvoid*)(sizeof(float) * 3)); // Normal

	glEnableVertexAttribArray(2); // Enable vertex texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), (GLvoid*)(sizeof(float) * 6)); // Texture

	glEnableVertexAttribArray(3); // Enable vertex tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), (GLvoid*)(sizeof(float) * 8)); // Tangent
	
	glEnableVertexAttribArray(4); // Enable vertex bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTNT), (GLvoid*)(sizeof(float) * 11)); // Bitangent

	// -----------------------------------------------------------------------------------------------
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------

template <>
void Mesh<VertexPTT>::setupVertexInput()
{
	// Generate the vertex array

	GLuint vertexBuffer, indexBuffer;
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(m_vertexArrayObject);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPTT) * m_vertexList.size(), (const void*)m_vertexList.data(), GL_STATIC_DRAW);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indexList.size(), (const void*)m_indexList.data(), GL_STATIC_DRAW);

	// Setup the attributes --------------------------------------------------------------------------
	glEnableVertexAttribArray(0); // Enable vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTT), 0); // Position

	glEnableVertexAttribArray(1); // Enable vertex texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTT), (GLvoid*)(sizeof(float) * 3)); // Texture

	glEnableVertexAttribArray(2); // Enable vertex tangent
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTT), (GLvoid*)(sizeof(float) * 5)); // Tangent

	// -----------------------------------------------------------------------------------------------
	glBindVertexArray(0);
}

// ----------------------------------------------------------------------------