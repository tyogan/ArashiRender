#include "framebuffer.h"

FrameBuffer::FrameBuffer(int width,int height)
	:mWidth(width),mHeight(height)
{
	mSSAO = new SSAO(mWidth, mHeight);
	glGenFramebuffers(1, &mFBO);
	glGenTextures(5, mRenderTextures);
	initFramebuffer();
}

FrameBuffer::~FrameBuffer()
{
	delete mSSAO;
}

void FrameBuffer::initFramebuffer()
{
	for (int i = 0; i < 5; i++)
	{
		glBindTexture(GL_TEXTURE_2D, mRenderTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	for (int i = 0; i < 5; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mRenderTextures[i], 0);
	}

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::vec3 FrameBuffer::getMIdxPixel(int x, int y)
{
	glm::vec3 mIdx(-1);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
	glReadBuffer(GL_COLOR_ATTACHMENT4);
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &mIdx);
	if (mIdx.z <0.001)
	{
		mIdx = glm::vec3(-1);
	}
	return mIdx;
}

void FrameBuffer::dump(void* dst)
{
	glBindTexture(GL_TEXTURE_2D, mRenderTextures[0]);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, dst);
}

void FrameBuffer::bindForDrawGBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	const GLenum COLOR_ATTACHMENTS[]
	{
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4
	};
	glDrawBuffers(4, COLOR_ATTACHMENTS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::bindForDrawImage()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}