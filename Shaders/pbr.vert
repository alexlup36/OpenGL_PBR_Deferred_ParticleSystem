#version 330 core

in layout(location = 0) vec3 vertexPosition;
in layout(location = 1) vec3 vertexNormal;
in layout(location = 2) vec2 vertexTexCoord;
in layout(location = 3) vec3 vertexTangent;
in layout(location = 4) vec3 vertexBitangent;

out VS_OUT
{
	vec2 texCoord;
	vec3 normalW;
	vec3 vertexW;
	mat3 TBNMatrix;
	vec3 viewPosTangent;
	vec3 fragmentPosTangent;
} vs_out;

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
	vs_out.TBNMatrix = transpose(mat3(t, b, n));
}

void main()
{
	// Calculate fragment position in screen space
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
	
	// Calculate vertex position in world coordinates
	vs_out.vertexW 	= vec3(model * vec4(vertexPosition, 1.0f));
	// Calculate normal direction in world coordinates
	vs_out.normalW 	= normalize(mat3(normalMat) * vertexNormal);
	// Pass the texture coordinates
	vs_out.texCoord	= vertexTexCoord;

	// Calculate the TNB matrix for normal mapping
	calculateTNBMatrix();

	// Calculate the view direction vector in tangent space
	vs_out.viewPosTangent = vs_out.TBNMatrix * viewPos;
	// Calculate the fragment position in tangent space
	vs_out.fragmentPosTangent = vs_out.TBNMatrix * vs_out.vertexW;
}