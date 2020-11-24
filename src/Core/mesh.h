#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <memory>

using std::vector;
using std::string;
using std::shared_ptr;

class CORE_API Mesh
{
public:
	static shared_ptr<Mesh> createCube();
	static shared_ptr<Mesh> createPlane();   
	static shared_ptr<Mesh> createSphere(float radius, unsigned int sectorCount, unsigned int stackCount);

public:
	vector<glm::vec3> mPositions;
	vector<glm::vec3> mNormals;
	vector<glm::vec2> mTexCoords;
	vector<unsigned int> mIndices;
};