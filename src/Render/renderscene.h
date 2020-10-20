#pragma once

#include "Core/scene.h"
#include "vao.h"
#include "shader.h"
#include "envmap.h"
#include "shadowmap.h"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

struct  MeshParam
{
	shared_ptr<VAO> mVAO;
	int mMatIdx;
	glm::mat4 mScale;
	glm::mat4 mRotate;
	glm::mat4 mTrans;
};

enum ModelType
{
	SPHERE,
	CUBE,
	PLANE
};

class RENDER_API RenderScene
{
public:
	void init();
	void addSceneMesh(string path,glm::mat4 size,glm::mat4 pos);
	void addSceneMesh(ModelType T, glm::mat4 size, glm::mat4 pos);
	void deleteSceneMesh(int meshIdx);

	void addLight(glm::vec3 lightDir, glm::vec3 lightColor);
	void setCamera(glm::vec3 pos);

private:
	void initScene();
	void initEnvmap();
	void initShadowmap();
	void initMaterials(string path);

public:
	shared_ptr<Scene> mScene;
	vector<MeshParam> mRenderMeshParam;
	shared_ptr<Envmap> mEnvmap;
	shared_ptr<Shadowmap> mShadowmap;
	vector<shared_ptr<ShaderProgram>> mMaterials;

	vector<vector<float>> mSHData;

	shared_ptr<ShaderProgram> mGBufferProgram;
};