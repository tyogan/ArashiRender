#include "render.h"
#include "importer.h"
#include "shadowmap.h"
#include "camera.h"

GLRender::GLRender()
{
	initFBO();
	mEnvmap.load("bin/envmap/envmap.jpg");
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, -3.f, 0));
	model = glm::scale(model, glm::vec3(0.3f));
	model = glm::rotate(model, 10.f, glm::vec3(0, 1.f, 0.f));
	mScene.loadObject("bin/model/nanosuit.obj", model);
	mScene.createCube(model);
}

GLRender::~GLRender()
{
}

GLuint GLRender::getTexture()
{
	return mTexture;
}

GLuint GLRender::render()
{
	renderShadow();
	mEnvmap.createCubemapTexture();
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 800, 600);
	renderBackground();
	renderObject();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 0;
}

void GLRender::renderBackground()
{
	glm::mat4 view = glm::mat4(glm::mat3(mScene.getCamera(0).getViewMat()));
	glm::mat4 proj = mScene.getCamera(0).getProjMat(45.f, 800.f / 600.f, 0.1f, 100.f);
	glDepthMask(GL_FALSE);
	mScene.getShader(0)->use();
	glActiveTexture(GL_TEXTURE0);
	mEnvmap.bindCreateCubeTexture();
	mScene.getShader(0)->setMat4f("V", view);
	mScene.getShader(0)->setMat4f("P", proj);
	mScene.getObject(0)->draw();
	mScene.getShader(0)->release();
	glDepthMask(GL_TRUE);
}

void GLRender::renderObject()
{

	glm::mat4 view, proj;
	view = mScene.getCamera(0).getViewMat();
	proj = mScene.getCamera(0).getProjMat(45.f, 800.f / 600.f, 0.1f, 100.f);

	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 shadowView = glm::lookAt(glm::vec3(-2.0f, 4.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glViewport(0, 0, 800, 600);

	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mShadowmap.getTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 1);

	mScene.getShader(2)->use();
	mScene.getShader(2)->setMat4f("V", view);
	mScene.getShader(2)->setMat4f("P", proj);
	mScene.getShader(2)->setMat4f("lightV", shadowView);
	mScene.getShader(2)->setMat4f("lightP", shadowProj);
	mScene.getShader(2)->setVec3("lightDir", mScene.getLight(0).getLightDir());
	mScene.getShader(2)->setVec3("viewPos", mScene.getCamera(0).getPos());
	for (int i = 1; i < mScene.getObjectSize(); i++)
	{
		mScene.getShader(2)->setMat4f("M", mScene.getObjectToworldMat(i));
		mScene.getObject(i)->draw();
	}
	mScene.getShader(2)->release();
}

void GLRender::renderShadow()
{
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 shadowView = glm::lookAt(glm::vec3(-2.0f, 4.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mShadowmap.bindForDraw();
	mScene.getShader(1)->use();
	mScene.getShader(1)->setMat4f("V", shadowView);
	mScene.getShader(1)->setMat4f("P", shadowProj);
	glCullFace(GL_FRONT);
	for (int i = 1; i < mScene.getObjectSize(); i++)
	{
		mScene.getShader(1)->setMat4f("M", mScene.getObjectToworldMat(i));
		mScene.getObject(i)->draw();
	}

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::initFBO()
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
}
