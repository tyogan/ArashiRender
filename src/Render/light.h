#pragma once

#include"glm\glm.hpp"

class RENDER_API Light
{
public:
	Light(glm::vec3 LightDir);
	glm::vec3 getLightDir()const;
private:
	glm::vec3 mLightDir;
};