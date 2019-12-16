#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>

using std::vector;
using std::string;

struct Vertex
{
	glm::vec3 mPos;
	glm::vec3 mNormal;
	glm::vec2 mTexCoords;
};

class RENDER_API Mesh
{
public:
	static Mesh createCube();
	static Mesh createSphere();
	static Mesh createPlane();   

	unsigned int getIndicesNums();
	unsigned int getVertexNums();

public:
	vector<Vertex> mVertices;
	vector<unsigned int> mIndices;
};