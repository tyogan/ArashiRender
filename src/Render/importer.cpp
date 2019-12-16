#include "importer.h"
#include <iostream>

void Importer::loadModel(char* path, vector<Mesh>& meshes)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	processNode(scene->mRootNode, scene, meshes);
}

void Importer::processNode(aiNode* node, const aiScene* scene, vector<Mesh>& meshes)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, meshes);
	}
}

Mesh Importer::processMesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh m;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 tmpVec;
		tmpVec.x = mesh->mVertices[i].x;
		tmpVec.y = mesh->mVertices[i].y;
		tmpVec.z = mesh->mVertices[i].z;
		vertex.mPos = tmpVec;
		tmpVec.x = mesh->mNormals[i].x;
		tmpVec.y = mesh->mNormals[i].y;
		tmpVec.z = mesh->mNormals[i].z;
		vertex.mNormal = tmpVec;
		if (mesh->mTextureCoords[0])
		{
			tmpVec.x = mesh->mTextureCoords[0][i].x;
			tmpVec.y = mesh->mTextureCoords[0][i].y;
			vertex.mTexCoords = glm::vec2(tmpVec.x, tmpVec.y);
		}
		else {
			vertex.mTexCoords = glm::vec2(0.f, 0.f);
		}
		m.mVertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m.mIndices.push_back(face.mIndices[j]);
		}
	}
	
	return m;
}
