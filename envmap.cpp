#include "envmap.h"

using std::vector;

Envmap::Envmap()
{
	initFBO();

	mCubeProgram = new ShaderProgram("bin/shader/cubemap_vert.glsl", "bin/shader/cubemap_frag.glsl");
	mCubeProgram->use();
	mCubeProgram->setInt("envmap", 0);
	mCubeProgram->release();
	mObject->createSphere(1.0, 64, 32);
}

Envmap::~Envmap()
{
	glDeleteTextures(1, &mCreateEnvCubemap);
	glDeleteTextures(1, &mCubeTexture);
	glDeleteTextures(1, &mImageTexture);
	delete mCubeProgram;
	delete mObject;
}

void Envmap::bindCubeTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeTexture);
}

void Envmap::bindCreateCubeTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCreateEnvCubemap);
}

void Envmap::bindImageTexture()
{
	glBindTexture(GL_TEXTURE_2D, mImageTexture);
}

void Envmap::createCubemapTexture()
{
	glm::mat4 captureProjection = glm::perspective(90.0f, 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glViewport(0, 0, 1024, 1024);
	mCubeProgram->use();
	mCubeProgram->setMat4f("P", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mImageTexture);
	for (unsigned int i = 0; i < 6; ++i)
	{
		mCubeProgram->setMat4f("V", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mCreateEnvCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		mObject->draw();
	}
	mCubeProgram->release();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Envmap::createIrradianceTexture()
{

}

void Envmap::load(std::string path)
{
	int width, height, nrChannels;
	Image<float> img;
	img.load(path);
	glBindTexture(GL_TEXTURE_2D, mImageTexture);
	if (img.data())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, img.width, img.height,
			0, GL_RGB, GL_FLOAT, img.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "texture failed to load at path: " << path << std::endl;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Envmap::load(const vector<std::string>& imgNames)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeTexture);
	int width, height, nrChannels;
	for (int i = 0; i < imgNames.size(); i++)
	{
		unsigned char* data = stbi_load(("bin/envmap/" + imgNames[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << imgNames[i] << std::endl;
			stbi_image_free(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Envmap::initFBO()
{
	glGenFramebuffers(1, &mFBO);
	glGenTextures(1, &mImageTexture);
	glGenTextures(1, &mCubeTexture);
	glGenTextures(1, &mCreateEnvCubemap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, mCreateEnvCubemap);
	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
			1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &mIrradianceTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mIrradianceTexture);
	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}