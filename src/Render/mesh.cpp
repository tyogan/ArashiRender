#include "mesh.h"
#include "vertex.h"

Mesh* Mesh::createSphere(float radius, unsigned int sectorCount, unsigned int stackCount)
{
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
	Mesh* m = new Mesh();
	Vertex vert;
	float sectorStep = 2 * M_PI / sectorCount;
	float stackStep = M_PI / stackCount;
	float sectorAngle, stackAngle;
	float lengthInv = 1.0 / radius;

	for (int i = 0; i <= stackCount; ++i)
	{
		float stackAngle = i * stackStep;
		float xz = radius * sinf(stackAngle);
		float y = radius * cosf(stackAngle);
		float ty = xz;
		float txz = y;

		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;
			float x = xz * cosf(sectorAngle);
			float z = xz * sinf(sectorAngle);
			vert.mPos.x = x;
			vert.mPos.y = y;
			vert.mPos.z = z;

			vert.mNormal.x = x * lengthInv;
			vert.mNormal.y = y * lengthInv;
			vert.mNormal.z = z * lengthInv;

			float s = (float)j / sectorCount * 180;
			float t = (float)i / stackCount * 180;
			vert.mTexCoords.x = s;
			vert.mTexCoords.y = t;
			m->mVertices.push_back(vert);
		}
	}

	for (int i = 0; i < stackCount; ++i)
	{
		int k1 = i * (sectorCount + 1);
		int k2 = k1 + sectorCount + 1;
		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				m->mIndices.push_back(k1);
				m->mIndices.push_back(k2);
				m->mIndices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1))
			{
				m->mIndices.push_back(k1 + 1);
				m->mIndices.push_back(k2);
				m->mIndices.push_back(k2 + 1);
			}
		}
	}
	m->mMat = 2;
	return m;
}

Mesh* Mesh::createPlane()
{
	Mesh* m = new Mesh();
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
		m->mVertices.push_back(vert);
	}
	
	for (unsigned int i = 0; i < sizeof(planeIndices) / sizeof(unsigned int); i++)
	{
		m->mIndices.push_back(planeIndices[i]);
	}
	m->mMat = 2;
	return m;
}

Mesh* Mesh::createCube()
{
	Mesh* m = new Mesh();
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
		m->mVertices.push_back(vert);
	}

	for (unsigned int i = 0; i < sizeof(cubeIndices) / sizeof(unsigned int); i++)
	{
		m->mIndices.push_back(cubeIndices[i]);
	}
	m->mMat = 2;
	return m;
}

unsigned int Mesh::getIndicesNums()const
{
	return mIndices.size();
}

unsigned int Mesh::getVertexNums()const
{
	return mVertices.size();
}

int Mesh::getMaterial()const
{
	return mMat;
}

void Mesh::setMaterial(int mat)
{
	mMat = mat;
}

//vector<Vertex>  Mesh::getVertices()const
//{
//
//}
//
//vector<unsigned int>  Mesh::getIndices()const
//{
//
//}