#pragma once
#include "camera.h"
#include "object.h"
#include "light.h"
#include "shader.h"

#include <map>
using std::map;

class RENDER_API Scene
{
public:
	Scene();
	~Scene();

	void loadObject(const char* path, glm::mat4 ObjectToWorldMat);
	void createCube(glm::mat4 ObjectToWorldMat);
	void creatSphere(glm::mat4 ObjectToWorldMat);
	void createPlane(glm::mat4 ObjectToWorldMat);
	Object* getObject(int idx)const;
	int getObjectSize()const;
	glm::mat4 getObjectToworldMat(int idx)const;

	void loadShader(const char* vertPath, const char* fragPath);
	ShaderProgram* getShader(int idx);

	void addCamera(Camera camera);
	void setCamera(int idx, Camera camera);
	Camera getCamera(int idx)const;

	void addLight(Light light);
	void setLight(int idx, Light light);
	Light getLight(int idx)const;

private:
	vector<Object*> mObjects;
	vector<Camera> mCameras;
	vector<Light> mLights;
	vector<ShaderProgram*> mShaders;
};