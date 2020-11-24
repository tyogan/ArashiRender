#include "vao.h"
#include "Core/mesh.h"

VAO::VAO()
{
	glGenVertexArrays(1, &mVAO);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1,&mVAO);
}

void VAO::create(shared_ptr<Mesh> mesh)
{
	mIdxNum = mesh->mIndices.size();
	unsigned int vtxNum = mesh->mPositions.size();

	GLuint vbo;
	GLuint ebo;

	glBindVertexArray(mVAO);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mIdxNum, mesh->mIndices.data(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * 2 + sizeof(glm::vec2))*vtxNum, nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*vtxNum, mesh->mPositions.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vtxNum, sizeof(glm::vec3)*vtxNum, mesh->mNormals.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vtxNum*2, sizeof(glm::vec2)*vtxNum, mesh->mTexCoords.data());

	glEnableVertexAttribArray(0);
	
	
	
	
	(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(sizeof(glm::vec3)*vtxNum));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(sizeof(glm::vec3)*vtxNum*2));
}

void VAO::sendShData(const vector<float>& data)
{
	glBindVertexArray(mVAO);
	GLuint sh;
	glGenBuffers(1, &sh);
	glBindBuffer(GL_ARRAY_BUFFER, sh);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float)*16, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(12 * sizeof(float)));
	//glVertexAttribPointer();

}

void VAO::draw()
{
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIdxNum, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}