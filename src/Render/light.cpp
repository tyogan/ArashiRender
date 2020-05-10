#include "Light.h"

Light::Light(glm::vec3 lightDir)
	:mLightDir(lightDir)
{}

glm::vec3 Light::getLightDir()const
{
	return mLightDir;
}