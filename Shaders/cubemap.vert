#version 330 core

layout (location = 0) in vec3 vPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 texCoords;

void main()
{
	// Transform position
	vec4 pos = projection * view * vec4(vPos, 1.0f);
	// Optimization - force the depth of each fragment to be 1 after the 
	// perspective division is done w/w=1. This way all the fragments of
	// the skybox that have something drawn in front of them will fail
	// the depth test and won't be rendered giving a boost in performance
	// We will also have to change the depth test function from the default
	// GL_LESS to GL_LEQUAL so that the depth test won't fail for depth values
	// of 1 and the skybox will be rendered where there is nothing in front of it.
    gl_Position = pos.xyww;
	// The position of each vertex is also a vector from the origin to
	// the vertex. So we can use it as texture coordinate to sample
	// the cube map in the fragment shader.
	texCoords = vPos;
}