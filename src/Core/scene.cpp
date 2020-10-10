#include "scene.h"

void Scene::loadMeshImport(string path)
{
	Importer im;
	vector<shared_ptr<Mesh>> m = im.loadModel(path.c_str());
	mMeshes.insert(mMeshes.end(), m.begin(), m.end());
}

void Scene::loadMeshSphere(int r)
{
	shared_ptr<Mesh> m = Mesh::createSphere(r, 64, 32);
	mMeshes.push_back(m);
}

void Scene::loadMeshPlane()
{
	shared_ptr<Mesh> m = Mesh::createPlane();
	mMeshes.push_back(m);
}

void Scene::loadMeshCube()
{
	shared_ptr<Mesh> m = Mesh::createCube();
	mMeshes.push_back(m);
}