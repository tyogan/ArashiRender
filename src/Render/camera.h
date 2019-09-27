#include <glm/glm.hpp>

class RENDER_API Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	~Camera();
	glm::mat4 getViewMat();
	glm::mat4 getProjMat(float fov,float aspect,float near,float far);
private:
	glm::vec3 m_Pos;
	glm::vec3 m_Target;
	glm::vec3 m_Up;
};