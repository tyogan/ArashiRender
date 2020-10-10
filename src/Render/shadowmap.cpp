#include "shadowmap.h"

Shadowmap::Shadowmap()
{
	mShadowProgram = shared_ptr<ShaderProgram>(new ShaderProgram("bin/shader/shadowmap_vert.glsl", "bin/shader/shadowmap_frag.glsl"));

	glGenFramebuffers(1, &mFramebuffer);

	glGenTextures(1, &mTextures);
	glBindTexture(GL_TEXTURE_2D, mTextures);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTextures, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Shadowmap::~Shadowmap()
{
	glDeleteFramebuffers(1, &mFramebuffer);
	glDeleteTextures(1, &mTextures);
}

void Shadowmap::bindForDraw(int idx)
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
}

GLuint Shadowmap::getShadowTexture()
{
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	return mTextures;
}