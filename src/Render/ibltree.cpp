#include "ibltree.h"

IBLTree::IBLTree(unsigned char* imageData, int width, int height, int channel)
{
	mImage.mWidth = width;
	mImage.mHeight = height;
	mImage.mChannel = channel;
	memcpy(mImage.mData.data(), imageData, width*height*channel);
}

IBLTree::~IBLTree()
{
	delete mpRoot;
}

void IBLTree::calEnergy(Node* node)
{

	for (int i = node->mBbox.mMin.x; i < node->mBbox.mMax.x; i++)
	{
		for (int j = node->mBbox.mMin.y; j < node->mBbox.mMax.y; j++)
		{
			node->mColor += mImage.mData[i*mImage.mWidth + j];
		}
	}
	node->mEnergy = 
		node->mColor.r * 0.212671f +
		node->mColor.g * 0.715160f +
		node->mColor.b * 0.072169f;
}

void IBLTree::buildTree(int depth)
{
	if (depth < 0 || mImage.mWidth < 0 || mImage.mHeight < 0) return;
	mpRoot = new Node();
	mpRoot->mBbox.mMin = glm::vec2(0, 0);
	mpRoot->mBbox.mMax = glm::vec2(mImage.mWidth, mImage.mHeight);
	mpRoot->mSubdAlex = SUBD_X_ALEX;
	mpRoot->mDepth = 1;

	mTreeDepth = depth;

}

void IBLTree::recursiveBuildNode(Node* node)
{
	if (node->mDepth == mTreeDepth)
	{
		calEnergy(node);
		return;
	}
	node->mpLeft = new Node();
	
	node->mpLeft->mDepth = node->mDepth += 1;
	if (node->mSubdAlex == SUBD_X_ALEX)
	{
		node->mpLeft->mBbox.mMin = node->mBbox.mMin;
		node->mpLeft->mBbox.mMax.x = node->mBbox.mMax.x / 2.0;
		node->mpLeft->mBbox.mMax.y = node->mBbox.mMax.y;
		node->mpLeft->mSubdAlex = SUBD_Y_ALEX;
	}
	else {
		node->mpLeft->mSubdAlex = SUBD_X_ALEX;
	}

	node->mpRight = new Node();

	recursiveBuildNode(node->mpLeft);
	recursiveBuildNode(node->mpLeft);

	node->mColor = node->mpLeft->mColor + node->mpRight->mColor;
	node->mEnergy = node->mpLeft->mEnergy + node->mpRight->mEnergy;
}

Sample* IBLTree::wrapSamples(int sampleNums, glm::vec2* pointsInput)
{
	Sample* samples = new Sample[sampleNums];
	glm::vec2* pPoints = new glm::vec2[sampleNums];
	mpRoot->mSampleNums = sampleNums;
	mpRoot->mSampleIndex = new int[sampleNums];
	
	mSampleCount = sampleNums;

	memcpy(pPoints, pointsInput, sizeof(glm::vec2)*sampleNums);

	for (int i = 0; i < sampleNums; i++)
	{
		mpRoot->mSampleIndex[i] = i;
	}

	findArea(pPoints, mpRoot, samples);

	delete[]pPoints;
	return samples;
}

void IBLTree::findArea(glm::vec2* points,Node* node,Sample* samples)
{
	if (node->mDepth == mTreeDepth||node->mSampleNums==1)
	{
		float fScale = node->mSampleNums > 1 ? 1.0 / node->mSampleNums : 1.0;
		glm::vec3 color = node->mColor*fScale;
		for (int i = 0; i < node->mSampleNums; ++i)
		{
			glm::vec2 poi = points[node->mSampleIndex[i]];
			Sample& s = samples[mSampleCount];

			s.mPos.x= node->mBbox.mMin.x + poi.x*(node->mBbox.mMax.x - node->mBbox.mMin.x);
			s.mPos.y = node->mBbox.mMin.y + poi.y*(node->mBbox.mMax.y - node->mBbox.mMin.y);
			s.mColor = color;
		}
		return;
	}

	float left = node->mpLeft->mEnergy / node->mEnergy;
	int* arrLeft = new int[node->mSampleNums];
	int* arrRight = new int[node->mSampleNums];
	int leftNum = 0;
	int rightNum = 0;

		
	if (node->mSubdAlex == SUBD_X_ALEX)
	{
		for (int i = 0; i < node->mSampleNums; i++)
		{
			glm::vec2& p = points[node->mSampleIndex[i]];
			if (p.x < left)
			{
				arrLeft[leftNum++] = node->mSampleIndex[i];
				p.x /= left;
			}
			else {
				arrRight[rightNum++] = node->mSampleIndex[i];
				p.x = (p.x - left) / (1.0 - left);
			}
		}
	}
	else {
		for (int i = 0; i < node->mSampleNums; i++)
		{
			glm::vec2& p = points[node->mSampleIndex[i]];
			if (p.y < left)
			{
				arrLeft[leftNum++] = node->mSampleIndex[i];
				p.y /= left;
			}
			else {
				arrRight[rightNum++] = node->mSampleIndex[i];
				p.y = (p.y - left) / (1.0 - left);
			}
		}
	}
	delete node->mSampleIndex;
	if (leftNum)
	{
		node->mpLeft->mSampleNums = leftNum;
		node->mpLeft->mSampleIndex = new int[leftNum];
		memcpy(node->mpLeft->mSampleIndex, arrLeft, sizeof(int)*leftNum);
	}
	if (rightNum)
	{
		node->mpRight->mSampleNums = rightNum;
		node->mpRight->mSampleIndex = new int[rightNum];
		memcpy(node->mpRight->mSampleIndex, arrRight, sizeof(int)*rightNum);
	}
	delete [] arrLeft;
	delete [] arrRight;
	if (leftNum) findArea(points, node->mpLeft, samples);
	if (rightNum) findArea(points, node->mpRight, samples);
}

void IBLTree::scaleImageColor()
{
#ifndef M_PI
#define M_PI 3.1415926
#endif // !M_PI

	int Width = mImage.mWidth;
	int Height = mImage.mHeight;

	float* fpFactor = new float[Height];
	int halfHeight = Height / 2;
	int sumScale = 0.0;
	for (int i = 0; i <= halfHeight; i++)
	{
		fpFactor[i] = fpFactor[Height - 1 - i] = cos((1.0 - (i + 0.5) / halfHeight)*M_PI*0.5f);
		sumScale += fpFactor[i];
	}

	sumScale = 4 * M_PI / (Width * sumScale * 2);
	for (int i = 0; i <= halfHeight; i++)
	{
		fpFactor[i] *= sumScale;
	}

	for (int j = 0; j < Height; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			mImage.mData[i*mImage.mWidth + j] *= fpFactor[j];
		}
	}
	delete []fpFactor;
}

