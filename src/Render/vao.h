#pragma once

#include "glad/glad.h"
#include "mesh.h"

class RENDER_API VAO
{
public:
	VAO();
	~VAO();

	void create(Mesh& mesh);
	void draw();
private:
	GLuint mVAO;
	GLuint mIdxNum;
};