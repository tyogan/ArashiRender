#pragma once

#include "glad/glad.h"

class RENDER_API VAO
{
public:
	VAO();
	~VAO();

	void create(const float* data, GLuint dataNum,const GLuint* indices,GLuint idxNum);
	void draw();
private:
	GLuint m_VAO;
	GLuint m_IdxNum;
};