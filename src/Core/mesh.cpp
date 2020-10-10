#include "mesh.h"
#include "vertex.h"

shared_ptr<Mesh> Mesh::createSphere(float radius, unsigned int sectorCount, unsigned int stackCount)
{

	const double M_PI = 3.14159265358979323846;
	shared_ptr<Mesh> m = std::make_shared<Mesh>();
	
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texcoord;

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
			pos = glm::vec3(x, y, z);
			m->mPositions.push_back(pos);

			normal = glm::vec3(x, y, z)*lengthInv;
			m->mNormals.push_back(normal);

			float s = (float)j / sectorCount * 180;
			float t = (float)i / stackCount * 180;
			texcoord = glm::vec2(s, t);
			m->mTexCoords.push_back(texcoord);
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
	return m;
}

shared_ptr<Mesh> Mesh::createPlane()
{
	shared_ptr<Mesh> m = std::make_shared<Mesh>();

	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texcoord;

	for (unsigned int i = 0; i < sizeof(planeVertices) / sizeof(float); i += 8)
	{
		pos = glm::vec3(planeVertices[i], planeVertices[i + 1], planeVertices[i + 2]);
		m->mPositions.push_back(pos);

		normal = glm::vec3(planeVertices[i + 3], planeVertices[i + 4], planeVertices[i + 5]);
		m->mNormals.push_back(normal);

		texcoord = glm::vec2(planeVertices[i + 6], planeVertices[i + 7]);
		m->mTexCoords.push_back(texcoord);
	}
	
	for (unsigned int i = 0; i < sizeof(planeIndices) / sizeof(unsigned int); i++)
	{
		m->mIndices.push_back(planeIndices[i]);
	}
	return m;
}

shared_ptr<Mesh> Mesh::createCube()
{
	shared_ptr<Mesh> m = std::make_shared<Mesh>();
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texcoord;

	for (unsigned int i = 0; i < sizeof(cubeVertices) / sizeof(float); i += 8)
	{
		pos = glm::vec3(cubeVertices[i], cubeVertices[i + 1], cubeVertices[i + 2]);
		m->mPositions.push_back(pos);

		normal = glm::vec3(cubeVertices[i + 3], cubeVertices[i + 4], cubeVertices[i + 5]);
		m->mNormals.push_back(normal);

		texcoord = glm::vec2(cubeVertices[i + 6], cubeVertices[i + 7]);
		m->mTexCoords.push_back(texcoord);
	}

	for (unsigned int i = 0; i < sizeof(cubeIndices) / sizeof(unsigned int); i++)
	{
		m->mIndices.push_back(cubeIndices[i]);
	}
	return m;
}