#include "Light.h"

#include <thread>

Light::Light(glm::vec3 lightDir,glm::vec3 color)
	:mLightDir(lightDir),mLightColor(color)
{}

glm::vec3 Light::getParallelLightDir()const
{
	return mLightDir;
}

glm::vec3 Light::getPointLightPos()const
{
	return mLightDir;
}

glm::vec3 Light::getLightColor()const
{
	return mLightColor;
}