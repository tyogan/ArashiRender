#include "shadowmap.h"

Shadowmap::Shadowmap()
{
	glGenFramebuffers(1, &mFramebuffer);
	glGenTextures(1, &mTextures);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mTextures);
	
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 1024, 1024, 16);

	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

Shadowmap::~Shadowmap()
{
	glDeleteFramebuffers(1, &mFramebuffer);
	glDeleteTextures(1, &mTextures);
}

void Shadowmap::bindForDraw(GLuint idx)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTextureS, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

GLuint Shadowmap::getTexture()
{
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	return mTextures;


	GLuint num;
	glGenBuffers(1, &num);
	glBindBuffer();
	glBufferData();
}