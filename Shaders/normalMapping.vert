#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec2 texCoord;
out vec3 vertexW;
out mat3 TBNMatrix;
out vec3 normalW;
out vec3 viewPosTangent;
out vec3 fragmentPosTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat;
uniform vec3 viewPos;

void calculateTNBMatrix()
{
	// Transform the normal, tangent and calculate binormals
	vec3 n = normalize(mat3(normalMat) * vertexNormal);
	vec3 t = normalize(mat3(normalMat) * vertexTangent);
	
	// Make sure the t and n vectors are orthogonal
	t = normalize(t - dot(t, n) * n);
	
	vec3 b = normalize(cross(t, n)); // Order important
	
	// Create the TBN matrix
	TBNMatrix = transpose(mat3(t, b, n));
}

void main()
{
	// Calculate fragment position in screen space
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
	
	// Calculate vertex position in world coordinates
	vertexW 	= vec3(model * vec4(vertexPosition, 1.0f));
	normalW 	= normalize(mat3(normalMat) * vertexNormal);
	
	// Pass the texture coordinates
	texCoord	= vertexTexCoord;
	
	// Calculate the TNB matrix for normal mapping
	calculateTNBMatrix();

	// Calculate the view direction vector in tangent space
	viewPosTangent = TBNMatrix * viewPos;
	// Calculate the fragment position in tangent space
	fragmentPosTangent = TBNMatrix * vertexW;
}