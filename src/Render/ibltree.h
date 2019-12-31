#pragma once
#include "glm/glm.hpp"
#include <vector>

#define SUBD_X_ALEX 0
#define SUBD_Y_ALEX 1

struct Sample
{
	glm::vec3 mPos;
	glm::vec3 mColor;
};

struct BBox
{
	glm::vec2 mMax;
	glm::vec2 mMin;
};

struct Image
{
	std::vector<int> mData;
	int mWidth;
	int mHeight;
	int mChannel;
};

class RENDER_API IBLTree
{
public:
	IBLTree(unsigned char* imageData,int width,int height,int channel);
	~IBLTree();

	class Node
	{
	public:
		int mDepth;
		int mSubdAlex;
		glm::vec3 mColor;
		float  mEnergy;
		BBox mBbox;
		Node* mpLeft;
		Node* mpRight;
		int mSampleNums;
		int* mSampleIndex;

		Node()
		{
			mDepth = 0;
			mSubdAlex = 0;
			mSampleNums = 0;
			mColor = glm::vec3(0);
			mEnergy = 0;
			mpLeft = mpRight = nullptr;
			mSampleIndex = nullptr;
		}
		~Node()
		{
			delete mpLeft;
			delete mpRight;
			delete mSampleIndex;
		}
	};

public:
	void buildTree(int depth);
	Sample* wrapSamples(int sampleNums,glm::vec2* points);

private:
	void scaleImageColor();
	void calEnergy(Node* node);
	
	void recursiveBuildNode(Node* node);
	void findArea(glm::vec2* point, Node* node, Sample* samples);

private:
	Image mImage;
	Node* mpRoot;
	int mTreeDepth;
	int mSampleCount;
};