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

uniform float t;
uniform int levels;
out float fLevels;
out vec4 eyeP;
out vec3 n;

void main()
{	

	// Normally, one would simply transform the vertex spatial position using 
	// gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0);
	
	// However in this lab we're going to play with the vertex position before this transformation
	vec3 p = inPosition;
	float og_Y = p.y;
	if(p.y > -0.6f){
		p.y += sin(p.y + t)/10.f;
		p.x += sin(p.x + t)/10.f;
		p.z += sin(p.z + t)/10.f;


		if(p.y <= -0.6f){
			p.y = -0.6f;
		}
	}

	/*if(p.y > 0){
		p.y += sin(p.y + t)/10.f;
		p.x += sin(p.x + t)/10.f;
		p.z += sin(p.z + t)/10.f;


		if(p.y <= 0){
			p.y = 0;
		}
	}

	else if(p.y <= 0 && p.y >= -0.5){
		if(p.x > 0){
			p.x -= sin(p.x + t)/5.f;
		}

		else{
			p.x += sin(p.x + t)/5.f;
		}

		if(p.z > 0){
			p.z -= sin(p.z + t)/5.f;
		}

		else{
			p.z += sin(p.z + t)/5.f;
		}
	
	}

	else if(p.y < -0.5){
		p.y -= sin(p.y + t)/10.f;
		p.x -= sin(p.x + t)/10.f;
		p.z -= sin(p.z + t)/10.f;


		if(p.y >= -0.5){
			p.y = -0.5;
		}
	}*/


	

	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(p, 1.0);

	fLevels = levels;

	// Get the vertex normal and vertex position in eye coordinates
	n = normalize(matrices.normalMatrix * inNormal);
	eyeP = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	
} 
