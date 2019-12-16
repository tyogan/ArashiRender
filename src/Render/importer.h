#pragma once

#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class RENDER_API Importer
{
public:
	void loadModel(char* path,vector<Mesh>& meshes);

private:
	void processNode(aiNode* node, const aiScene* scene, vector<Mesh>& meshes);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};