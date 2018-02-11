#version 330 core

in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec3 vertexNormal;
in layout(location = 2) vec2 vertexTexCoord;
in layout(location = 3) vec3 vertexTangent;
in layout(location = 4) vec3 vertexBitangent;

out vec2 texCoord;
out vec3 vertexW;
out mat3 TBNMatrix;
out vec3 normalW;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat;

void calculateTNBMatrix()
{
	// Transform the normal, tangent and calculate binormals
	vec3 n = normalize(vec3(normalMat * vec4(vertexNormal, 0.0f)));
	vec3 t = normalize(vec3(normalMat * vec4(vertexTangent, 0.0f)));
	
	// Make sure the t and n vectors are orthogonal
	t = normalize(t - dot(t, n) * n);
	
	vec3 b = normalize(cross(t, n)); // Order important
	
	// Create the TBN matrix
	TBNMatrix = mat3(t, b, n);
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
}