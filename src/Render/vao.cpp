#include "vao.h"

VAO::VAO()
	:mIdxNum(0)
{
	glGenVertexArrays(1, &mVAO);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1,&mVAO);
}

void VAO::create(Mesh& mesh)
{
	mIdxNum = mesh.getIndicesNums();
	GLuint vbo;
	GLuint ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mIdxNum, mesh.mIndices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mesh.getVertexNums(),mesh.mVertices.data(),GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTexCoords));
}

void VAO::draw()
{
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIdxNum, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}