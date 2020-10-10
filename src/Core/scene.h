#pragma once
#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "importer.h"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

class CORE_API Scene
{
public:
	void loadMeshImport(string path);
	void loadMeshSphere(int r);
	void loadMeshPlane();
	void loadMeshCube();

public:
	vector<shared_ptr<Mesh>> mMeshes;
	vector<shared_ptr<Light>> mLights;
	shared_ptr<Camera> mCamera;
};