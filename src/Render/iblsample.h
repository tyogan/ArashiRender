#pragma once
#include "glm/glm.hpp"
#include <vector>

using std::vector;

struct Sample
{
	glm::vec2 mPos;
	glm::vec3 mColor;
};

struct Image
{
	std::vector<int> mData;
	int mWidth;
	int mHeight;
	int mChannel;
};

class RENDER_API IBLSample
{
public:
	IBLSample(unsigned char* data, int width, int height, int nrChannel);
private:
	Image mImage;
	float* mImageBright;
};