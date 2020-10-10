#pragma once 
#include "glad/glad.h"

#include <iostream>

class RENDER_API FrameBuffer
{
public:
	FrameBuffer(int width,int height);
	~FrameBuffer();
	void dump(void* dst);
	unsigned int getTexture();
	void bindForDraw();
private:
	unsigned int mFBO;
	unsigned int mTexture;
public:
	int mWidth;
	int mHeight;
};