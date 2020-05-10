#include "object.h"

void Object::loadModel(const char* path)
{
	Importer im;
	im.loadModel(path);
	MeshAndVAO tmpVAOStruct;
	for (auto it : im.getMeshes())
	{
		
		VAO* tmpVAO=new VAO();
		tmpVAO->create(*it);
		tmpVAOStruct.pMesh = it;
		tmpVAOStruct.pVAO = tmpVAO;
		tmpVAOStruct.mat = 0;
		mVAOs.push_back(tmpVAOStruct);
	}
}

void Object::createCube()
{
	Mesh* tmpCube = Mesh::createCube();
	VAO* tmpVAO = new VAO();
	MeshAndVAO tmpVAOStruct;
	tmpVAO->create(*tmpCube);
	tmpVAOStruct.pMesh = tmpCube;
	tmpVAOStruct.pVAO = tmpVAO;
	tmpVAOStruct.mat = 0;
	mVAOs.push_back(tmpVAOStruct);
}

void Object::createSphere(float radius, unsigned int sectorCount, unsigned int stackCount)
{
	Mesh* tmpSphere = Mesh::createSphere(radius, sectorCount, stackCount);
	VAO* tmpVAO = new VAO();
	MeshAndVAO tmpVAOStruct;
	tmpVAO->create(*tmpSphere);
	tmpVAOStruct.pMesh = tmpSphere;
	tmpVAOStruct.pVAO = tmpVAO;
	tmpVAOStruct.mat = 0;
	mVAOs.push_back(tmpVAOStruct);
}

void Object::createPlane()
{
	Mesh* tmpPlane = Mesh::createPlane();
	VAO* tmpVAO = new VAO();
	MeshAndVAO tmpVAOStruct;
	tmpVAO->create(*tmpPlane);
	tmpVAOStruct.pMesh = tmpPlane;
	tmpVAOStruct.pVAO = tmpVAO;
	tmpVAOStruct.mat = 0;
	mVAOs.push_back(tmpVAOStruct);
}

void Object::draw()const
{
	for (auto it : mVAOs)
	{
		it.pVAO->draw();
	}
}

void Object::setOjectMaterial(const int& index,const int& mat)
{
	mVAOs[index].mat = mat;
}

int Object::getObjectMaterial(const int& index)const
{
	return mVAOs[index].mat;
}

void Object::setObjectToWorldMat(const glm::mat4& matrix)
{
	mObjectToWorldMat = matrix;
}

glm::mat4 Object::getObjectToWorldMat()const
{
	return mObjectToWorldMat;
}

void Object::setObjectScale(const glm::mat4& scale)
{
	mObjectScale = scale;
}