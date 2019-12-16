#include "mesh.h"
#include "vertex.h"

Mesh Mesh::createSphere()
{
	Mesh m;
	return m;
}

Mesh Mesh::createPlane()
{
	Mesh m;
	Vertex vert;
	for (unsigned int i = 0; i < sizeof(planeVertices) / sizeof(float); i += 8)
	{
		vert.mPos.x = planeVertices[i];
		vert.mPos.y = planeVertices[i + 1];
		vert.mPos.z = planeVertices[i + 2];

		vert.mNormal.x = planeVertices[i + 3];
		vert.mNormal.y = planeVertices[i + 4];
		vert.mNormal.z = planeVertices[i + 5];

		vert.mTexCoords.x = planeVertices[i + 6];
		vert.mTexCoords.y = planeVertices[i + 7];
		m.mVertices.push_back(vert);
	}
	
	for (unsigned int i = 0; i < sizeof(planeIndices) / sizeof(unsigned int); i++)
	{
		m.mIndices.push_back(planeIndices[i]);
	}

	return m;
}

Mesh Mesh::createCube()
{
	Mesh m;
	Vertex vert;
	for (unsigned int i = 0; i < sizeof(cubeVertices) / sizeof(float); i += 8)
	{
		vert.mPos.x = cubeVertices[i];
		vert.mPos.y = cubeVertices[i + 1];
		vert.mPos.z = cubeVertices[i + 2];

		vert.mNormal.x = cubeVertices[i + 3];
		vert.mNormal.y = cubeVertices[i + 4];
		vert.mNormal.z = cubeVertices[i + 5];

		vert.mTexCoords.x = cubeVertices[i + 6];
		vert.mTexCoords.y = cubeVertices[i + 7];
		m.mVertices.push_back(vert);
	}

	for (unsigned int i = 0; i < sizeof(cubeIndices) / sizeof(unsigned int); i++)
	{
		m.mIndices.push_back(cubeIndices[i]);
	}

	return m;
}

unsigned int Mesh::getIndicesNums()
{
	return mIndices.size();
}

unsigned int Mesh::getVertexNums()
{
	return mVertices.size();
}