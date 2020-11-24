#pragma once

#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
using std::vector;

class CORE_API Importer
{
public:
	vector<shared_ptr<Mesh>> loadModel(const char* path);

private:
	void processNode(aiNode* node, const aiScene* scene);
	shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

private:
	vector<shared_ptr<Mesh>> mMeshes;
};