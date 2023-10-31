#include "Common.h"
#include "tetrahedron.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


Ctetrahedron::Ctetrahedron()
{
    m_vao = 0;

}

Ctetrahedron::~Ctetrahedron()
{}


// Create the plane, including its geometry, texture mapping, normal, and colour
void Ctetrahedron::Create(string directory, string filename)
{
    // Load the texture
    m_texture.Load(directory + filename, true);

    m_directory = directory;
    m_filename = filename;

    // Set parameters for texturing using sampler object
    m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    //// Use VAO to store state associated with vertices
    //glGenVertexArrays(1, &m_vao);
    //glBindVertexArray(m_vao);

    // Create a VBO
    //m_vbo.Create();
    //m_vbo.Bind();

    if (m_vao == 0)
    {
        GLfloat vertices[] = {

            //Position			//Texcoords	    //Normals
           //Triangle front                     
           0.f, 1.0f, 0.f,		0.5f, 1.f,		0.f, 0.f, 1.f,
           -1.0f, -1.0f, 1.0f,	0.f, 0.f,		0.f, 0.f, 1.f,
           1.0f, -1.0f, 1.0f,	    1.f, 0.f,		0.f, 0.f, 1.f,

           //Triangle left         
           0.f, 1.0f, 0.f,		0.5f, 1.f,		-1.f, 0.f, 0.f,
           -1.0f, -1.0f, -1.0f,   0.f, 0.f,		-1.f, 0.f, 0.f,
           -1.0f, -1.0f, 1.0f,	1.f, 0.f,		-1.f, 0.f, 0.f,

           //Triangle back         
           0.f, 1.0f, 0.f,		0.5f, 1.f,		0.f, 0.f, -1.f,
           1.0f, -1.0f, -1.0f,	0.f, 0.f,		0.f, 0.f, -1.f,
          -1.0f, -1.0f, -1.0f,   1.f, 0.f,		0.f, 0.f, -1.f,

          //Triangles right       
          0.f, 1.0f, 0.f,		0.5f, 1.f,		1.f, 0.f, 0.f,
          1.0f, -1.0f, 1.0f,	    0.f, 0.f,		1.f, 0.f, 0.f,
          1.0f, -1.0f, -1.0f,	1.f, 0.f,		1.f, 0.f, 0.f,



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
    }
    glBindVertexArray(m_vao);




}

// Render the plane as a triangle strip
void Ctetrahedron::Render()
{

    glBindVertexArray(m_vao);
    m_texture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);


}

// Release resources
void Ctetrahedron::Release()
{
    m_texture.Release();

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &vbo);
}