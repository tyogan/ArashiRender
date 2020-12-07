#include "renderscene.h"

RenderScene::~RenderScene()
{
	delete mScene;
	delete mEnvmap;
	delete mEnvShadowmap;
	delete mShadowmap;
	delete mGBufferProgram;
}

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
	mGBufferProgram = new ShaderProgram("bin/shader/gbuffer_vert.glsl", "bin/shader/gbuffer_frag.glsl");
}

void RenderScene::initScene()
{
	mScene = new Scene();
	
	this->addSceneMesh("bin/model/teapot.obj", glm::mat4(1),glm::mat4(1));
	this->addSceneMesh(ModelType::PLANE, glm::mat4(1), glm::mat4(1));
	
	Light* light = new Light();
	light->mLightColor = glm::vec3(0.5f);
	light->mLightDir = glm::vec3(-2.0f, 4.0f, 1.0f);
	this->addLight(light);
	light = nullptr;

	Camera* cam = new PerspectiveCamera();
	cam->mPos = glm::vec3(0, 0, 10);
	this->setCamera(cam);
	cam = nullptr;
}

void RenderScene::initEnvmap()
{
	mEnvmap = new Envmap();
	mEnvmap->init();
	mEnvmap->load("bin/envmap/alex.hdr");
}

void RenderScene::initShadowmap()
{
	mEnvShadowmap = new Shadowmap();
	mEnvShadowmap->resize(64, 1024);
	mShadowmap = new Shadowmap();
	mShadowmap->resize(mScene->mLights.size(), 1024);
}

void RenderScene::initMaterials(string path)
{
	shared_ptr<ShaderProgram> s0(new ShaderProgram((path + "phong_vert.mtl").c_str(), (path + "phong_frag.mtl").c_str()));
	shared_ptr<ShaderProgram> s1(new ShaderProgram((path + "phongIBL_vert.mtl").c_str(), (path + "phongIBL_frag.mtl").c_str()));
	shared_ptr<ShaderProgram> s2(new ShaderProgram((path + "phongSH_vert.mtl").c_str(), (path + "phongSH_frag.mtl").c_str()));

	mMaterialLibraries.push_back(s0);
	mMaterialLibraries.push_back(s1);
	mMaterialLibraries.push_back(s2);
}

void RenderScene::addSceneMesh(string path, glm::mat4 size, glm::mat4 pos)
{
	int len1 = mScene->mMeshes.size();
	mScene->loadMeshImport(path);

	for (auto iter = mScene->mMeshes.begin() + len1; iter != mScene->mMeshes.end(); iter++)
	{
		MeshParam m;
		m.mMatIdx = 1;
		m.mTrans = pos;
		m.mScale = size;
		m.mRotate = glm::mat4(1.f);
		m.mObjectName = "Object";

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
	MeshParam m;

	switch (T)
	{
	case SPHERE:
		mScene->loadMeshSphere(1);
		m.mObjectName = "Sphere";
		break;
	case CUBE:
		mScene->loadMeshCube();
		m.mObjectName = "Cube";
		break;
	case PLANE:
		mScene->loadMeshPlane();
		m.mObjectName = "Plane";
		break;
	default:
		break;
	}
	
	m.mMatIdx = 1;
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

void RenderScene::addLight(Light* light)
{
	mScene->mLights.push_back(shared_ptr<Light>(light));
}

void RenderScene::setCamera(Camera* cam)
{
	mScene->mCamera = shared_ptr<Camera>(cam);
}