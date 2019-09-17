#include "vao.h"

VAO::VAO()
{
	glGenVertexArrays(1, &m_VAO);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1,&m_VAO);
}

void VAO::create(const float* data,const int* indices, int idxNum)
{
	GLuint vbo;
	GLuint ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*idxNum, indices, GL_STATIC_DRAW);


}