#include "renderscene.h"

void RenderScene::init()
{
	if (!gladLoadGL()) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		exit(-1);
	}
	initScene();
	initEnvmap();
	initShadowmap();
	initMaterials("bin/mtl/");
	mGBufferProgram = shared_ptr<ShaderProgram>(new ShaderProgram("bin/shader/gbuffer_vert.glsl", "bin/shader/gbuffer_frag.glsl"));
}

void RenderScene::initScene()
{
	mScene = shared_ptr<Scene>(new Scene());
	addSceneMesh("bin/model/teapot.obj", glm::mat4(1),glm::mat4(1));
	addSceneMesh(ModelType::PLANE, glm::mat4(1), glm::mat4(1));
	addLight(glm::vec3(-2.0f, 4.0f, 1.0f), glm::vec3(0.5f));
	setCamera(glm::vec3(0, 0, 10));
}

void RenderScene::initEnvmap()
{
	mEnvmap = shared_ptr<Envmap>(new Envmap);
	mEnvmap->init();
	mEnvmap->load("bin/envmap/alex.hdr");
}

void RenderScene::initShadowmap()
{
	mShadowmap = shared_ptr<Shadowmap>(new Shadowmap);
}

void RenderScene::initMaterials(string path)
{
	shared_ptr<ShaderProgram> s0(new ShaderProgram((path + "obj_vert.mtl").c_str(), (path + "obj_frag.mtl").c_str()));
	shared_ptr<ShaderProgram> s1(new ShaderProgram((path + "phong_vert.mtl").c_str(), (path + "phong_frag.mtl").c_str()));
	shared_ptr<ShaderProgram> s2(new ShaderProgram((path + "phongIBL_vert.mtl").c_str(), (path + "phongIBL_frag.mtl").c_str()));
	shared_ptr<ShaderProgram> s3(new ShaderProgram((path + "phongSH_vert.mtl").c_str(), (path + "phongSH_frag.mtl").c_str()));

	mMaterials.push_back(s0);
	mMaterials.push_back(s1);
	mMaterials.push_back(s2);
	mMaterials.push_back(s3);
}

void RenderScene::addSceneMesh(string path, glm::mat4 size, glm::mat4 pos)
{
	int len1 = mScene->mMeshes.size();
	mScene->loadMeshImport(path);

	for (auto iter = mScene->mMeshes.begin() + len1; iter != mScene->mMeshes.end(); iter++)
	{
		MeshParam m;
		m.mMatIdx = 3;
		m.mTrans = pos;
		m.mScale = size;
		m.mRotate = glm::mat4(1.f);

		m.mVAO = shared_ptr<VAO>(new VAO);
		m.mVAO->create(*iter);

		mRenderMeshParam.push_back(m);
	}
	mSHData = SphericalHarmonics::computeSceneSHTrans(mScene);
	for (int i = 0; i < mScene->mMeshes.size(); i++)
	{
		mRenderMeshParam[i].mVAO->sendShData(mSHData[i]);
	}
}

void RenderScene::addSceneMesh(ModelType T, glm::mat4 size, glm::mat4 pos)
{
	switch (T)
	{
	case SPHERE:
		mScene->loadMeshSphere(1);
		break;
	case CUBE:
		mScene->loadMeshCube();
		break;
	case PLANE:
		mScene->loadMeshPlane();
		break;
	default:
		break;
	}

	MeshParam m;
	m.mMatIdx = 3;
	m.mTrans = pos;
	m.mScale = size;

	m.mVAO = shared_ptr<VAO>(new VAO);
	m.mVAO->create(*(mScene->mMeshes.end()-1));

	mRenderMeshParam.push_back(m);

	mSHData = SphericalHarmonics::computeSceneSHTrans(mScene);
	for (int i = 0; i < mScene->mMeshes.size(); i++)
	{
		mRenderMeshParam[i].mVAO->sendShData(mSHData[i]);
	}
}

void RenderScene::deleteSceneMesh(int meshIdx)
{

}

void RenderScene::addLight(glm::vec3 lightDir, glm::vec3 lightColor)
{
	mScene->mLights.push_back(shared_ptr<Light>(new Light(lightDir, lightColor)));
}

void RenderScene::setCamera(glm::vec3 pos)
{
	mScene->mCamera = shared_ptr<Camera>(new PerspectiveCamera);
	mScene->mCamera->mPos = pos;
}