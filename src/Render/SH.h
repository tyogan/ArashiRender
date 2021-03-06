#pragma once

#include <glm/glm.hpp>
#include "Utility/image.h"
#include "Utility/samplefunc.h"
#include "Core/scene.h"

#include <vector>

using std::vector;

class Scene;

class RENDER_API SphericalHarmonics
{
public:	
	static std::vector<glm::vec3>				
		computeLightSHCoeff(const Image<float>& envmap, unsigned int order = 3);

	static vector<vector<float>>
		computeSceneSHTrans(Scene* scene, unsigned int order = 3);
};