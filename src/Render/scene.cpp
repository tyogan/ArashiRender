#include "scene.h"

Scene::Scene()
{
	Camera cam(glm::vec3(0, 0, 10.f), glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));
	mCameras.push_back(cam);
	Light light(glm::vec3(-2.0f, 4.0f, 1.0f));
	mLights.push_back(light);
	createCube(glm::mat4(1));
	loadShader("bin/shader/envmap_vert.glsl", "bin/shader/envmap_frag.glsl");
	loadShader("bin/shader/shadowmap_vert.glsl", "bin/shader/shadowmap_frag.glsl");
	loadShader("bin/shader/obj_vert.glsl", "bin/shader/obj_frag.glsl");
}

Scene::~Scene()
{
}

void Scene::loadObject(const char* path, glm::mat4 ObjectToWorldMat)
{
	Object* obj = new Object();
	obj->loadModel(path);
	mObjects.push_back(obj);
	obj->setObjectToWorldMat(ObjectToWorldMat);
}

void Scene::createCube(glm::mat4 ObjectToWorldMat)
{
	Object* obj = new Object();
	obj->createCube();
	mObjects.push_back(obj);
	obj->setObjectToWorldMat(ObjectToWorldMat);
}

void Scene::creatSphere(glm::mat4 ObjectToWorldMat)
{
	Object* obj = new Object();
	obj->createSphere(1,64,32);
	mObjects.push_back(obj);
	obj->setObjectToWorldMat(ObjectToWorldMat);
}

void Scene::createPlane(glm::mat4 ObjectToWorldMat)
{
	Object* obj = new Object();
	obj->createPlane();
	mObjects.push_back(obj);
	obj->setObjectToWorldMat(ObjectToWorldMat);
}

Object* Scene::getObject(int idx)const
{
	return mObjects[idx];
}

int Scene::getObjectSize()const
{
	return mObjects.size();
}

glm::mat4 Scene::getObjectToworldMat(int idx)const
{
	return mObjects[idx]->getObjectToWorldMat();
}

void Scene::loadShader(const char* vertPath, const char* fragPath)
{
	ShaderProgram* shader = new ShaderProgram(vertPath, fragPath);
	mShaders.push_back(shader);
}

ShaderProgram* Scene::getShader(int idx)
{
	return mShaders[idx];
}

void Scene::addCamera(Camera camera)
{
	mCameras.push_back(camera);
}

void Scene::setCamera(int idx, Camera camera)
{
	mCameras[idx] = camera;
}

Camera Scene::getCamera(int idx)const
{
	return mCameras[idx];
}

void Scene::addLight(Light light)
{
	mLights.push_back(light);
}

void Scene::setLight(int idx, Light light)
{
	mLights[idx] = light;
}

Light Scene::getLight(int idx)const
{
	return mLights[idx];
}