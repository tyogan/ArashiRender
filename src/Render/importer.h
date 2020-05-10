#pragma once

#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

class RENDER_API Importer
{
public:
	void loadModel(const char* path);
	std::vector<Mesh*> getMeshes()const;
private:
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh*> mMeshes;
};