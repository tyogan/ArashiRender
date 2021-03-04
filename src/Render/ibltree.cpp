#include "ibltree.h"

#include <iostream>
IBLTree::IBLTree(int nDepth, Image<float> image)
{
	mRoot = NULL;
	mImage = image;
	ImageColorScale(mImage);
	BuildTree(nDepth);
	std::cout << "hhhh";
}

IBLTree::~IBLTree()
{
	SAFE_DELETE(mRoot);
}

void IBLTree::ImageColorScale(Image<float> &image)
{
	int Height = mImage.height;
	int Width = mImage.width;
	float *fpFactor = new float[Height];
	float fHeight = Height / 2.f;
	float fAreaSalar = 0.f;
	for (int y = 0; y < Height / 2; y++)
	{
		fpFactor[y] = fpFactor[Height - 1 - y] = cos((1.f - (y + .5f) / fHeight) * 1.570795f);
		fAreaSalar += fpFactor[y];
	}
	fAreaSalar = 4 * 3.1415926f / (fAreaSalar * 2 * Width);
	for (int y = 0; y < Height; y++)
	{
		fpFactor[y] *= fAreaSalar;
	}

	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			mImage(x, y, 0) *= fpFactor[y];
			mImage(x, y, 1) *= fpFactor[y];
			mImage(x, y, 2) *= fpFactor[y];
		}
	}

	delete[] fpFactor;
}

void IBLTree::BuildTree(int nDepth)
{
	if (nDepth <= 0 || mImage.width <= 0 || mImage.height <= 0)
	{
		return;
	}
	
	mTreeDepth = nDepth;
	mRoot = new TNode;
	mRoot->mDepth = 1;
	mRoot->mBBox.min.x = mRoot->mBBox.min.y = 0.f;
	mRoot->mBBox.max.x = mRoot->mBBox.max.y = 1.f;
	mRoot->mSubdAxle = SUBD_X_AXLE;
	
	RecursiveSubdivision(mRoot);
}

void IBLTree::RecursiveSubdivision(TNode *pNode)
{
	if (pNode->mDepth == mTreeDepth)
	{
		RetreveNodeEnergy(pNode);
		return;
	}

	pNode->mLeft = new TNode;
	pNode ->mRight = new TNode;
	
	pNode->mLeft->mDepth = pNode->mRight->mDepth = pNode->mDepth + 1;
	pNode->mLeft->mSubdAxle = pNode->mRight->mSubdAxle =
		(pNode->mSubdAxle == SUBD_X_AXLE) ? SUBE_Y_AXLE : SUBD_X_AXLE;
	pNode->mLeft->mBBox = pNode->mRight->mBBox = pNode->mBBox;
	if (pNode->mSubdAxle == SUBD_X_AXLE)
	{
		pNode->mLeft->mBBox.max.x = pNode->mRight->mBBox.min.x =
			(pNode->mBBox.max.x + pNode->mBBox.min.x)*.5f;
	}
	else {
		pNode->mLeft->mBBox.max.y = pNode->mRight->mBBox.min.y =
			(pNode->mBBox.max.y + pNode->mBBox.min.y)*.5f;
	}

	RecursiveSubdivision(pNode->mLeft);
	RecursiveSubdivision(pNode->mRight);

	pNode->mColor = pNode->mLeft->mColor + pNode->mRight->mColor;
	pNode->mEnergy = pNode->mLeft->mEnergy + pNode->mRight->mEnergy;
}

void IBLTree::RetreveNodeEnergy(TNode *pNode)
{
	int nX0 = (int)(pNode->mBBox.min.x*mImage.width + .5f);
	int nX1 = (int)(pNode->mBBox.max.x*mImage.width + .5f);
	int nY0 = (int)(pNode->mBBox.min.y*mImage.height + .5f);
	int nY1 = (int)(pNode->mBBox.max.y*mImage.height + .5f);

	pNode->mColor = glm::vec3(0.f);
	for (int x = nX0; x < nX1; x++)
	{
		for (int y = nY0; y < nY1; y++)
		{
			
			glm::vec3 color = glm::vec3(mImage(x, y, 0), mImage(x, y, 1), mImage(x, y, 2));
			pNode->mColor += color;
		}
	}
	pNode->mEnergy=
		pNode->mColor.r * 0.212671f +
		pNode->mColor.g * 0.715160f +
		pNode->mColor.b * 0.072169f;
}

CSample *IBLTree::sampleWraping(int nSamples, glm::vec2 *pInputPos)
{
	if (nSamples <= 0 || pInputPos == NULL)
	{
		return NULL;
	}

	glm::vec2 *lpPoints = new glm::vec2[nSamples];
	CSample *lpSample = new CSample[nSamples];
	mRoot->mSampleIdx = new int[nSamples];
	mRoot->mSampleNum = nSamples;
	for (int i = 0; i < nSamples; i++)
	{
		mRoot->mSampleIdx[i] = i;
	}

	memcpy(lpPoints, pInputPos, sizeof(glm::vec2)*nSamples);

	mGlobalIndex = 0;
	RecursiveWarping(mRoot, lpPoints, lpSample);


	delete [] lpPoints;
	return lpSample;
}

void IBLTree::RecursiveWarping(TNode *pNode, glm::vec2 *pPos, CSample *pSample)
{
	if (pNode->mSampleNum == 1 || pNode->mLeft == NULL)
	{
		glm::vec3 color;
		float fScale = (pNode->mSampleNum > 1) ? 1.f / pNode->mSampleNum : 1.f;
		color = pNode->mColor*fScale;

		for (int i = 0; i < pNode->mSampleNum; i++)
		{
			glm::vec2 lPos= pPos[pNode->mSampleIdx[i]];
			CSample *lpSamp = &pSample[mGlobalIndex++];
			lpSamp->pos.x = pNode->mBBox.min.x +
				(pNode->mBBox.max.x - pNode->mBBox.min.x)*lPos.x;
			lpSamp->pos.y = pNode->mBBox.min.y +
				(pNode->mBBox.max.y - pNode->mBBox.min.y)*lPos.y;
			lpSamp->color = color;
		}
		return;
	}

	float fSubd = pNode->mLeft->mEnergy / pNode->mEnergy;
	int *npLeft = new int[pNode->mSampleNum];
	int *npRight = new int[pNode->mSampleNum];
	int nLeft = 0;
	int nRight = 0;
	if (pNode->mSubdAxle == SUBD_X_AXLE)
	{
		for (int i = 0; i < pNode->mSampleNum; i++)
		{
			int k = pNode->mSampleIdx[i];
			glm::vec2 *lpPos = pPos + k;
			if (lpPos->x < fSubd)
			{
				npLeft[nLeft++] = pNode->mSampleIdx[i];
				lpPos->x /= fSubd;
			}
			else {
				npRight[nRight++] = pNode->mSampleIdx[i];
				lpPos->x = (lpPos->x - fSubd) / (1.f - fSubd);
			}
		}
	}
	else {
		for (int i = 0; i < pNode->mSampleNum; i++)
		{
			glm::vec2 *lpPos = pPos + pNode->mSampleIdx[i];
			if (lpPos->y < fSubd)
			{
				npLeft[nLeft++] = pNode->mSampleIdx[i];
				lpPos->y /= fSubd;
			}
			else {
				npRight[nRight++] = pNode->mSampleIdx[i];
				lpPos->y = (lpPos->y - fSubd) / (1 - fSubd);
			}

		}
	}

	SAFE_DELETE_ARRAY(pNode->mSampleIdx);
	if (nLeft)
	{
		pNode->mLeft->mSampleIdx = new int[nLeft];
		memcpy(pNode->mLeft->mSampleIdx, npLeft, sizeof(int) * nLeft);
		pNode->mLeft->mSampleNum = nLeft;
	}
	if (nRight)
	{
		pNode->mRight->mSampleIdx = new int[nRight];
		memcpy(pNode->mRight->mSampleIdx, npRight, sizeof(int)*nRight);
		pNode->mRight->mSampleNum = nRight;
	}
	SAFE_DELETE_ARRAY(npLeft);
	SAFE_DELETE_ARRAY(npRight);
	if (nLeft) RecursiveWarping(pNode->mLeft, pPos, pSample);
	if (nRight) RecursiveWarping(pNode->mRight, pPos, pSample);
}