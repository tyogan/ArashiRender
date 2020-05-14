#pragma once

#include"glm\glm.hpp"

class RENDER_API Light
{
public:
	Light(glm::vec3 light,glm::vec3 color);
	glm::vec3 getParallelLightDir()const;
	glm::vec3 getPointLightPos()const;
	glm::vec3 getLightColor()const;
	
private:
	glm::vec3 mParallelLightDir;
	glm::vec3 mPointLightPos;
	glm::vec3 mLightColor;
};