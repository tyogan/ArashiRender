#pragma once 
#include "glad/glad.h"
#include <glm/glm.hpp>

#include "ssao.h"

#include <iostream>

class RENDER_API FrameBuffer
{
public:
	FrameBuffer(int width,int height);
	~FrameBuffer();

	void bindForDrawGBuffer();
	void bindForDrawImage();
	void dump(void* dst);

	glm::vec3 getMIdxPixel(int x, int y);
	
private:
	void initFramebuffer();

public:
	int mWidth;
	int mHeight;

	union
	{
		GLuint mRenderTextures[5];
		struct
		{
			GLuint color, position, normal, texcoord, midx;
		};
	};

	SSAO* mSSAO;

private:
	GLuint mFBO;
};