#include "ssao.h"

SSAO::SSAO(GLuint width,GLuint height)
	:mWidth(width),mHeight(height)
{
	mSSAOProgram = new ShaderProgram("bin/shader/ssao_vert.glsl", "bin/shader/ssao_frag.glsl");
	mSSAOBlurProgram = new ShaderProgram("bin/shader/ssao_vert.glsl", "bin/shader/ssaoblur_frag.glsl");

	mSSAOProgram->setInt("gPositionDepth", 0);
	mSSAOProgram->setInt("gNormal", 1);
	mSSAOProgram->setInt("texNoise", 2);

	glGenFramebuffers(1, &mFBO);
	glGenTextures(2, mTextures);

	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	for (int i = 0; i < 2; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTextures[i], 0);
	}
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	sampleKernel();
}

SSAO::~SSAO()
{
	delete mSSAOProgram;
	delete mSSAOBlurProgram;
}

void SSAO::sampleKernel()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // Ëæ»ú¸¡µãÊý£¬·¶Î§0.0 - 1.0
	std::default_random_engine generator;
	for (GLuint i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		GLfloat scale = GLfloat(i) / 64.0;
		scale = Samplefunc::lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		mKernel.push_back(sample);
	}

	vector<glm::vec3> ssaoNoise;
	for (GLuint i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
		mSSAOProgram->setVec3("samples[" + std::to_string(i) + "]", mKernel[i]);
	}

	glGenTextures(1, &mNoiseTexture);
	glBindTexture(GL_TEXTURE_2D, mNoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAO::bindForCreate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint SSAO::getTexture()
{
	return ssaoTex;
}

GLuint SSAO::getBlurTexture()
{
	return ssaoBlurTex;
}