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

struct MeshParam
{
	shared_ptr<VAO> mVAO;
	string mObjectName;
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
	~RenderScene();
	void init();

	void addSceneMesh(string path,glm::mat4 size,glm::mat4 pos);
	void addSceneMesh(ModelType T, glm::mat4 size, glm::mat4 pos);
	void deleteSceneMesh(int meshIdx);
	void addLight(Light* light);
	void setCamera(Camera* cam);

private:
	void initScene();
	void initEnvmap();
	void initShadowmap();
	void initMaterials(string path);
	void initSSAO();

public:
	vector<vector<float>> mSHData;

	Scene* mScene;
	vector<MeshParam> mRenderMeshParam;
	
	Envmap* mEnvmap;
	Shadowmap* mEnvShadowmap;
	Shadowmap* mShadowmap;

	ShaderProgram* mGBufferProgram;
	vector<shared_ptr<ShaderProgram>> mMaterialLibraries;
};