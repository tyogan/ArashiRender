#include "envmap.h"

Envmap::Envmap()
{
}

Envmap::~Envmap()
{
	glDeleteTextures(1, &mLoadImageTex);
	glDeleteTextures(1, &mCubeTex);
}

void Envmap::init()
{
	mRenderBgProgram = shared_ptr<ShaderProgram>(
		new ShaderProgram("bin/shader/renderbg_vert.glsl", "bin/shader/renderbg_frag.glsl"));
	
	//cubeVAO
	shared_ptr<Mesh> cubeMesh = Mesh::createCube();
	mCubeVAO = shared_ptr<VAO>(new VAO);
	mCubeVAO->create(cubeMesh);

	//texture
	glGenTextures(1, &mLoadImageTex);
	glGenTextures(1, &mCubeTex);

	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeTex);
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
}

void Envmap::drawBackground(glm::mat4 view,glm::mat4 proj)
{
	glDepthMask(GL_FALSE);
	mRenderBgProgram->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeTex);
	mRenderBgProgram->setMat4f("V", view);
	mRenderBgProgram->setMat4f("P", proj);
	mCubeVAO->draw();
	mRenderBgProgram->release();
	glDepthMask(GL_TRUE);
}

void Envmap::load(std::string path)
{
	int width, height, nrChannels;
	Image<float> img;
	img.load(path);
	glBindTexture(GL_TEXTURE_2D, mLoadImageTex);
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

	createCubeTexture();
	mSHLight = SphericalHarmonics::computeLightSHCoeff(img);
}

void Envmap::createCubeTexture()
{
	shared_ptr<ShaderProgram> createCubeTexProgram = shared_ptr<ShaderProgram>(
		new ShaderProgram("bin/shader/createcubemap_vert.glsl", "bin/shader/createcubemap_frag.glsl"));
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

	GLuint FBO, rbo;
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glViewport(0, 0, 1024, 1024);
	createCubeTexProgram->use();
	createCubeTexProgram->setMat4f("P", captureProjection);
	createCubeTexProgram->setInt("envmap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mLoadImageTex);
	for (unsigned int i = 0; i < 6; ++i)
	{
		createCubeTexProgram->setMat4f("V", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mCubeTex, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mCubeVAO->draw();
	}
	createCubeTexProgram->release();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &FBO);
}