#pragma once
#include <glm/glm.hpp>

class CORE_API Camera
{
public:
	glm::mat4 getViewMat()const;
	virtual glm::mat4 getProjMat()const = 0;
	void setProjMat(glm::mat4 view);

public:
	float mZNear = 0.1f;
	float mZFar = 100.f;

	glm::vec3 mPos = glm::vec3(10, 0, 0);
	glm::vec3 mTarget = glm::vec3(0, 0, 0);
	glm::vec3 mUp = glm::vec3(0, 1, 0);
};

class CORE_API PerspectiveCamera :public Camera
{
public:
	float mFov = 60.f;
	float mAspect = 1.f;

public:
	virtual glm::mat4 getProjMat()const;
};

class CORE_API OrthogonalCamera :public Camera
{
public:
	float mLeft = -1.f;
	float mRight = 1.f;
	float mBottom = -1.f;
	float mTop = 1.f;

public:
	virtual glm::mat4 getProjMat()const;
};