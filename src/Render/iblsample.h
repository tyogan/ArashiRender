#pragma once
#include "glm/glm.hpp"
#include "Utility/image.h"

#include <vector>
using std::vector;

struct Sample
{
	glm::vec2 mPos;
	glm::vec3 mColor;
};

class RENDER_API IBLSample
{
public:
	IBLSample(Image<float> image);
	~IBLSample();
	Sample* sampleImage(int nums,glm::vec2* pnts);

private:
	int findRow(int uEnery);
	int findColumn(int row, int vEnery);

private:
	Image<float> mImage;
	vector<vector<float>> mImageBright;
	vector<float> mImageEnerySum;
};