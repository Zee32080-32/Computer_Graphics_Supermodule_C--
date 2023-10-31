#pragma once

#include "Common.h"
#include "Texture.h"
#include "VertexBufferObject.h"

// Class for generating a unit cube
class Ccube
{
public:
	Ccube();
	~Ccube();
	void Create(string filename);
	void Create(string sDirectory, string sFilename);

	void Render();
	void Release();
private:
	UINT m_vao;
	GLuint vbo;

	GLuint m_uiVAO;
	CVertexBufferObject m_VBO;
	CTexture m_tTexture;

	string m_directory;
	string m_filename;
};

