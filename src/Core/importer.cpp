#include "importer.h"
#include <iostream>

vector<shared_ptr<Mesh>> Importer::loadModel(const char* path)
{
	vector<shared_ptr<Mesh>> Meshes;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return Meshes;
	}
	processNode(scene->mRootNode, scene,Meshes);
	return Meshes;
}

void Importer::processNode(aiNode* node, const aiScene* scene,vector<shared_ptr<Mesh>>& Meshes)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, Meshes);
	}
}

shared_ptr<Mesh> Importer::processMesh(aiMesh* mesh, const aiScene* scene)
{
	shared_ptr<Mesh> m=std::make_shared<Mesh>();

	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texcoord;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		m->mPositions.push_back(pos);

		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		m->mNormals.push_back(normal);

		if (mesh->mTextureCoords[0])
		{
			texcoord.x = mesh->mTextureCoords[0][i].x;
			texcoord.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			texcoord = glm::vec2(0.f, 0.f);
		}
		m->mTexCoords.push_back(texcoord);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m->mIndices.push_back(face.mIndices[j]);
		}
	}
	return m;
}