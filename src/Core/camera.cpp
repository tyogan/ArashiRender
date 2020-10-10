#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getViewMat()const
{
	return glm::lookAt(mPos, mTarget, mUp);
}

glm::mat4 PerspectiveCamera::getProjMat()const
{
	return glm::perspective(mFov, mAspect, mZNear, mZFar);
}

glm::mat4 OrthogonalCamera::getProjMat()const
{
	return glm::ortho(mLeft, mRight, mBottom, mTop, mZNear, mZFar);
}
