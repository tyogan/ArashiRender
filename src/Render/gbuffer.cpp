#include "gbuffer.h"

GBuffer::GBuffer(GLuint width, GLuint height)
{
	glGenFramebuffers(1, &mFBO);
	glGenTextures(3, mTextures);
}

GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1, &mFBO);
	glDeleteBuffers(3, mTextures);
}

void GBuffer::bindForDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	for (int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, mTextures[i], 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	GLuint attachments[3] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2
	};
	glDrawBuffers(3, attachments);
}

