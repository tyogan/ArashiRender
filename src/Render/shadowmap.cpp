#include "shadowmap.h"

Shadowmap::Shadowmap()
{
	glGenFramebuffers(1, &mFramebuffer);
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
	glDeleteTextures(1, &mTexture);
}

void Shadowmap::bindForDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

GLuint Shadowmap::readTexture()
{
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	return mTexture;
}