#pragma once

#include "glad/glad.h"

class VAO
{
public:
	VAO();
	~VAO();

	void create(const float* data,const int* indices,int idxNum);
	void draw();
private:
	unsigned int m_VAO;
};