#include "render.h"
#include "importer.h"
#include "shadowmap.h"
#include "camera.h"

#include "stb_image.h"

GLRender::GLRender()
{
	glGenFramebuffers(1, &mFBO);
	
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
	
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	initShader();
	initVAO();
}

GLRender::~GLRender()
{
	delete mObjShader;
	delete mShadowShader;
	delete mBgShader;
}

GLuint GLRender::getTexture()
{
	return mTexture;
}

GLuint GLRender::render()
{
	renderShadow();
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderBackground();
	renderObject();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 0;
}

void GLRender::renderGBuffer()
{

}

void GLRender::renderBackground()
{
	Camera cam(glm::vec3(0, 0, 10.f), glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));
	glm::mat4 view = glm::mat4(glm::mat3(cam.getViewMat()));
	glm::mat4 proj = cam.getProjMat(45.f, 800.f / 600.f, 0.1f, 100.f);

	glViewport(0, 0, 800, 600);
	glDepthMask(GL_FALSE);
	mBgShader->use();
	glActiveTexture(GL_TEXTURE0);
	mEnvmap.bindCubeTexture();
	mBgShader->setMat4f("V", view);
	mBgShader->setMat4f("P", proj);
	mBgVAO->draw();
	glDepthMask(GL_TRUE);
	mBgShader->release();
}

void GLRender::renderObject()
{
	Camera cam(glm::vec3(0, 0, 10.f), glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));
	glm::mat4 model, view, proj;
	model = glm::translate(model, glm::vec3(0, -3.f, 0));
	model = glm::scale(model, glm::vec3(0.3f));
	model = glm::rotate(model, 10.f, glm::vec3(0, 1.f, 0.f));
	view = cam.getViewMat();
	proj = cam.getProjMat(45.f, 800.f / 600.f, 0.1f, 100.f);

	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 shadowView = glm::lookAt(glm::vec3(-2.0f, 4.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glViewport(0, 0, 800, 600);
	
	glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mObjShader->use();
	mObjShader->setMat4f("M", model);
	mObjShader->setMat4f("V", view);
	mObjShader->setMat4f("P", proj);
	mObjShader->setMat4f("lightV", shadowView);
	mObjShader->setMat4f("lightP", shadowProj);
	mObjShader->setVec3("lightDir", glm::vec3(-2.0f, 4.0f, 1.0f));
	mObjShader->setVec3("viewPos", cam.getPos());
	for (int i = 0; i < mVAOs.size(); i++)
	{
		mVAOs[i]->draw();
	}
	mObjShader->release();
}

void GLRender::renderShadow()
{;
	glm::mat4 shadowModel;
	shadowModel = glm::translate(shadowModel, glm::vec3(0, -3.f, 0));
	shadowModel = glm::scale(shadowModel, glm::vec3(0.3f));
	shadowModel = glm::rotate(shadowModel, 10.f, glm::vec3(0, 1.f, 0.f));
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 shadowView = glm::lookAt(glm::vec3(-2.0f, 4.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mShadowmap.bindForDraw();
	mShadowShader->use();
	mShadowShader->setMat4f("M", shadowModel);
	mShadowShader->setMat4f("V", shadowView);
	mShadowShader->setMat4f("P", shadowProj);
	glCullFace(GL_FRONT);
	for (int i = 0; i < mVAOs.size(); i++)
	{
		mVAOs[i]->draw();
	}

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mShadowmap.getTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 1);
}

void GLRender::initShader()
{
	mObjShader = new ShaderProgram("bin/shader/obj_vert.glsl", "bin/shader/obj_frag.glsl");
	mObjShader->use();
	mObjShader->setInt("shadowmap", 0);
	mObjShader->setInt("texImage", 1);

	mShadowShader = new ShaderProgram("bin/shader/shadowmap_vert.glsl", "bin/shader/shadowmap_frag.glsl");
	mBgShader = new ShaderProgram("bin/shader/envmap_vert.glsl", "bin/shader/envmap_frag.glsl");
	mBgShader->setInt("skybox", 0);

	vector<string> imgNames
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	mEnvmap.load(imgNames);
}

void GLRender::initVAO()
{
	for (int i = 0; i < mVAOs.size(); i++)
	{
		delete mVAOs[i];
	}

	Mesh cubeMesh = Mesh::createCube();

	mBgVAO = new VAO();
	mBgVAO->create((cubeMesh));

	VAO* cubeVAO = new VAO();
	cubeVAO->create(cubeMesh);
	mVAOs.push_back(cubeVAO);

	vector<Mesh> bunnyMeshes;
	Importer im;
	im.loadModel("bin/model/nanosuit.obj", bunnyMeshes);

	VAO* tmpVAO;
	for (unsigned int i = 0; i < bunnyMeshes.size(); i++)
	{
		tmpVAO = new VAO();
		tmpVAO->create(bunnyMeshes[i]);
		mVAOs.push_back(tmpVAO);
	}
}