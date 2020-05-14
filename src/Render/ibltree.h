#pragma once
#define SUBD_X_AXLE 0
#define SUBE_Y_AXLE 1

#define SAFE_DELETE(p)			{ if (p) delete (p);   (p) = NULL; }
#define SAFE_DELETE_ARRAY(p)	{ if (p) delete [](p); (p) = NULL; }

#include <glm/glm.hpp>
#include "image.h"

struct CSample
{
	glm::vec2 pos;
	glm::vec3 color;
};

struct CBBox
{
	glm::vec2 min, max;
};

class IBLTree
{
public:
	class TNode
	{
	public:
		int       mSubdAxle;
		int       mDepth;
		float     mEnergy;
		glm::vec3 mColor;
		int      *mSampleIdx;
		int       mSampleNum;
		CBBox     mBBox;
		
		TNode *mLeft, *mRight;
		TNode()
		{
			mSubdAxle = mDepth = 0;
			mEnergy = 0.f;
			mColor = glm::vec3(0.f);
			mSampleIdx = NULL;
			mSampleNum = 0;
			mLeft = mRight = NULL;
		}
		~TNode()
		{
			SAFE_DELETE(mLeft);
			SAFE_DELETE(mRight);
			SAFE_DELETE(mSampleIdx);
		}
	};

public:
	IBLTree(int nDepth, Image<float> image);
	~IBLTree(void);
	CSample *sampleWraping(int nSamples, glm::vec2 *pInputPos);

private:
	void BuildTree(int nDepth);
	void RetreveNodeEnergy(TNode *pNode);
	void RecursiveSubdivision(TNode *pNode);
	void ImageColorScale(Image<float> &image);

	void RecursiveWarping(TNode *pNode, glm::vec2 *pPoints, CSample *pSample);
private:
	Image<float> mImage;
	TNode *mRoot;
	int    mTreeDepth;
	int    mGlobalIndex;
};