#include "vao.h"

VAO::VAO()
	:m_IdxNum(0)
{
	glGenVertexArrays(1, &m_VAO);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1,&m_VAO);
}

void VAO::create(const float* data, GLuint dataNum,const GLuint* indices, GLuint idxNum)
{
	m_IdxNum = idxNum;
	GLuint vbo;
	GLuint ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*idxNum, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*dataNum,data,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

void VAO::draw()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_IdxNum, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void fun(float* data,int dataNum)
{
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*dataNum, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
