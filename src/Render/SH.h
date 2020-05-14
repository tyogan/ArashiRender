#pragma once

#include <glm/glm.hpp>
#include "image.h"
#include "scene.h"

#include <vector>

class RENDER_API SphericalHarmonics
{
public:	
	
	static std::vector<glm::vec3>				
		computeLightSHCoeff(const Image<glm::vec3>* envmap, unsigned int order = 3);

	static std::vector<std::vector<float>>		
		computeSceneSHCoeff(const Scene* scene, unsigned int order = 3);

};