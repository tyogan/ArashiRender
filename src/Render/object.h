#pragma once
#include "vao.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "importer.h"
#include "mesh.h"

#include <map>
#include <vector>
using std::map;
using std::vector;

class RENDER_API Object
{
public:
	void loadModel(const char* path);
	void createCube();
	void createSphere(float radius, unsigned int sectorCount, unsigned int stackCount);
	void createPlane();

	void draw()const;

	void setMeshMaterial(const int& index, const int& mat);
	int getMeshMaterial(const int& index)const;
	int getMeshSize()const;
	vector<Vertex> getMeshVertices(int index)const;
	vector<unsigned int> getMeshIndices(int index)const;

	int getMeshVertexsSize(int index)const;
	int getMeshIndexSize(int index)const;

	void setObjectToWorldMat(const glm::mat4& matrix);
	glm::mat4 getObjectToWorldMat()const;
	void setObjectScale(const glm::mat4& scale);
private:
	struct MeshAndVAO
	{
		Mesh* pMesh;
		VAO* pVAO;
	};
	vector<MeshAndVAO> mVAOs;
	glm::mat4 mObjectToWorldMat;
	glm::mat4 mObjectScale;
};