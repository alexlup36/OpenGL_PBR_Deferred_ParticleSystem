#version 330 core

// Direction vector representing a 3D texture coordinate
in vec3 texCoords;

// Output colour
out vec4 fragmentColor;

// Cube map texture sampler
uniform samplerCube envMap;

void main()
{
    fragmentColor = texture(envMap, texCoords);
}