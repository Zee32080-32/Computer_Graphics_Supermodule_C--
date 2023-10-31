#include "cube.h"

Ccube::Ccube()
{
	m_vao = 0;
}

Ccube::~Ccube()
{
	Release();
}

void Ccube::Create(string directory, string filename)
{
	// Load the texture
	m_tTexture.Load(directory + filename, true);

	m_directory = directory;
	m_filename = filename;

	// Set parameters for texturing using sampler object
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glGenVertexArrays(1, &m_uiVAO);
	//glBindVertexArray(m_uiVAO);
	//m_VBO.Create();
	//m_VBO.Bind();
	if (m_vao == 0)
	{

		GLfloat vertices[] =
		{
			// Back face
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
			// Front face
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
			// Left face
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			// Right face
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			// Top face
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left      
		};

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &vbo);
		// Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Link vertex attributes
		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	};
	glBindVertexArray(m_vao);


	//// Write the code to add interleaved point, texture coord, and normal of the cube
	//glm::vec3 v0 = glm::vec3(-1, -1, 1);
	//glm::vec3 v1 = glm::vec3(1, -1, 1);
	//glm::vec3 v2 = glm::vec3(1, 1, 1);
	//glm::vec3 v3 = glm::vec3(-1, 1, 1);
	//glm::vec3 n = glm::vec3(0, 0, 1);
	//glm::vec2 t0 = glm::vec2(0, 0);
	//glm::vec2 t1 = glm::vec2(1, 0);
	//glm::vec2 t2 = glm::vec2(1, 1);
	//glm::vec2 t3 = glm::vec2(0, 1);

	//m_VBO.AddData(&v0, sizeof(glm::vec3));
	//m_VBO.AddData(&t0, sizeof(glm::vec2));
	//m_VBO.AddData(&n, sizeof(glm::vec3));
	//m_VBO.AddData(&v1, sizeof(glm::vec3));
	//m_VBO.AddData(&t1, sizeof(glm::vec2));
	//m_VBO.AddData(&n, sizeof(glm::vec3));
	//m_VBO.AddData(&v3, sizeof(glm::vec3));
	//m_VBO.AddData(&t3, sizeof(glm::vec2));
	//m_VBO.AddData(&n, sizeof(glm::vec3));
	//m_VBO.AddData(&v2, sizeof(glm::vec3));
	//m_VBO.AddData(&t2, sizeof(glm::vec2));
	//m_VBO.AddData(&n, sizeof(glm::vec3));

	//// Upload data to GPU
	//m_VBO.UploadDataToGPU(GL_STATIC_DRAW);
	//GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	//// Vertex positions
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	//// Texture coordinates
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	//// Normal vectors
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


}







void Ccube::Render()
{
	glBindVertexArray(m_vao);
	m_tTexture.Bind();
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}

void Ccube::Release()
{
	m_tTexture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_VBO.Release();
}
