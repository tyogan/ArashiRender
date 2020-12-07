#include "envmap.h"

Envmap::~Envmap()
{
	glDeleteTextures(1, &mEnvmapImageTex);
	glDeleteTextures(1, &mEnvmapCubeTex);
}

void Envmap::init()
{
	mRenderBgProgram = shared_ptr<ShaderProgram>(
		new ShaderProgram("bin/shader/renderbg_vert.glsl", "bin/shader/renderbg_frag.glsl"));
	mCreateCubeTexProgram = shared_ptr<ShaderProgram>(
		new ShaderProgram("bin/shader/createcubemap_vert.glsl", "bin/shader/createcubemap_frag.glsl"));
	//cubeVAO
	shared_ptr<Mesh> cubeMesh = Mesh::createCube();
	mCubeVAO = shared_ptr<VAO>(new VAO);
	mCubeVAO->create(cubeMesh);

	//texture
	glGenTextures(1, &mEnvmapImageTex);
	glGenTextures(1, &mEnvmapCubeTex);

	glBindTexture(GL_TEXTURE_CUBE_MAP, mEnvmapCubeTex);
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, mEnvmapCubeTex);
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
	glBindTexture(GL_TEXTURE_2D, mEnvmapImageTex);
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
	
	int treeDepth = 8;
	int sampleNum = 128;
	glm::vec2* pnts = Samplefunc::samplePoints(sampleNum);
	
	IBLTree tree(treeDepth, img);
	CSample* bgSample = tree.sampleWraping(sampleNum, pnts);
	float u, v;
	for (int i = 0; i < 128; i++)
	{
		u = bgSample[i].pos.x;
		v = bgSample[i].pos.y;
		double phi = 2 * 3.1415926 * u;
		double theta = acos(1 - 2 * v);
		double r = sin(theta);
		EnvmapLight tmpLight;
		tmpLight.mDir = -glm::vec3(r * cos(phi), cos(theta), r * sin(phi));
		tmpLight.mColor = glm::vec3(bgSample[i].color);
		mEnvLights.push_back(tmpLight);
	}
	delete[] bgSample;
	delete[] pnts;
}

void Envmap::createCubeTexture()
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

	GLuint FBO, rbo;
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glViewport(0, 0, 1024, 1024);
	mCreateCubeTexProgram->use();
	mCreateCubeTexProgram->setMat4f("P", captureProjection);
	mCreateCubeTexProgram->setInt("envmap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mEnvmapImageTex);
	for (unsigned int i = 0; i < 6; ++i)
	{
		mCreateCubeTexProgram->setMat4f("V", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mEnvmapCubeTex, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mCubeVAO->draw();
	}
	mCreateCubeTexProgram->release();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &FBO);
}