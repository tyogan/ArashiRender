#include "shadowmap.h"

Shadowmap::Shadowmap()
	:mLayerNums(0), mSize(0)
{
	mShadowProgram = shared_ptr<ShaderProgram>(new ShaderProgram("bin/shader/shadowmap_vert.glsl", "bin/shader/shadowmap_frag.glsl"));

	glGenFramebuffers(1, &mFramebuffer);
	glGenTextures(1, &mTextures);
}

Shadowmap::~Shadowmap()
{
	glDeleteFramebuffers(1, &mFramebuffer);
	glDeleteTextures(1, &mTextures);
}

void Shadowmap::resize(GLuint layers, GLuint size)
{
	if (mLayerNums == layers && mSize == size)
	{
		return;
	}

	mLayerNums = layers;
	mSize = size;

	glDeleteTextures(1, &mTextures);

	glGenTextures(1, &mTextures);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mTextures);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16, mSize, mSize, mLayerNums, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Shadowmap::bindForDraw(int idx)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glViewport(0, 0, 1024, 1024);
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTextures, 0, idx);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

GLuint Shadowmap::getShadowTexture()
{
	return mTextures;
}