#pragma once

#include "glad/glad.h"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

class Mesh;
class CORE_API VAO
{
public:
	VAO();
	~VAO();

	void create(shared_ptr<Mesh> mesh);
	void draw();

	void sendShData(const vector<float>& data);

private:
	GLuint mVAO;
	GLuint mIdxNum;
};