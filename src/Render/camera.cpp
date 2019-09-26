#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
	:m_Pos(pos), m_Target(target), m_Up(up)
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMat()
{
	return glm::lookAt(m_Pos, m_Target, m_Up);
}

glm::mat4 Camera::getProjMat(float fov, float aspect, float near, float far)
{
	return glm::perspective(glm::radians(fov), aspect, near, far);
}
