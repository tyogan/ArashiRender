#include "iblsample.h"
#include <iostream>

IBLSample::IBLSample(Image<float> image)
	:mImageBright(vector<vector<float>>(image.height,vector<float>(image.width,0)))
	,mImageEnerySum(vector<float>(image.height,0))
	, mImage(image)
{
	
	float enery = 0;
	for (int h = 0; h < mImage.height; h++)
	{
		for (int w = 0; w < mImage.width; w++)
		{
			if (w == 0)
			{
				enery = mImage(0, h, 0) * 0.212671f +
					mImage(0, h, 1) * 0.715160f +
					mImage(0, h, 2) * 0.072169f;
				mImageBright[h][0] = enery;
			}
			else {
				enery = mImage(w, h, 0) * 0.212671f +
					mImage(w, h, 1) * 0.715160f +
					mImage(w, h, 2) * 0.072169f;
				mImageBright[h][w] = enery + mImageBright[h][w - 1];
			}
			
		}
	}

	mImageEnerySum[0] = mImageBright[0].back();
	for (int h = 1; h < image.height; h++)
	{
		mImageEnerySum[h] = mImageEnerySum[h-1] + mImageBright[h].back();
	}
}

IBLSample::~IBLSample()
{
}

Sample* IBLSample::sampleImage(int nums, glm::vec2* pnts)
{
	Sample* samples = new Sample[nums];

	for (int i = 0; i < nums; i++)
	{
		glm::vec2 poi = pnts[i];
		float uEnery = poi[0]*mImageEnerySum.back();
		float row = findRow(uEnery);
		float vEnery = poi[1] * mImageBright[row].back();
		float column = findColumn(row, vEnery);
		samples[i].mPos = glm::vec2(column, row);
		samples[i].mColor = glm::vec3(mImage(column, row, 0), mImage(column, row, 1), mImage(column, row, 2));
	}
	return samples;
}

int IBLSample::findRow(int uEnery)
{
	for (int i = 0; i < mImageEnerySum.size(); i++)
	{
		if (uEnery <= mImageEnerySum[i])
		{
			return i;
		}
	}
}

int IBLSample::findColumn(int row, int vEnery)
{
	for (int i = 0; i < mImageBright[row].size(); i++)
	{
		if (vEnery <= mImageBright[row][i])
		{
			return i;
		}
	}
}
