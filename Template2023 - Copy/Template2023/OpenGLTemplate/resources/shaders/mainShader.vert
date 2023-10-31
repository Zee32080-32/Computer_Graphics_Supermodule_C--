#version 400 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;


// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec2 vTexCoord;	// Texture coordinate

uniform int levels;
out float fLevels;
out vec4 P;
out vec3 n;
out vec3 worldPosition;	// used for skybox


void main()
{	
	worldPosition = inPosition;
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0);
	
	fLevels = levels;

	vTexCoord = inCoord;
	
	n = normalize(matrices.normalMatrix * inNormal);
	P = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
} 
