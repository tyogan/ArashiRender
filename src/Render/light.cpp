#include "Light.h"

Light::Light(glm::vec3 lightDir,glm::vec3 color)
	:mParallelLightDir(lightDir),mPointLightPos(lightDir),mLightColor(color)
{}

glm::vec3 Light::getParallelLightDir()const
{
	return mParallelLightDir;
}

glm::vec3 Light::getPointLightPos()const
{
	return mPointLightPos;
}

glm::vec3 Light::getLightColor()const
{
	return mLightColor;
}