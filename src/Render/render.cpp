#include "render.h"
#include <time.h>

GLRender::GLRender(RenderScene* renderScene)
	:mRenderScene(renderScene)
{
	struct SHBlock
	{
		glm::vec3 data[16];
	} block;

	for (int i = 0; i < mRenderScene->mEnvmap->mSHLight.size(); i++)
	{
		block.data[i] = mRenderScene->mEnvmap->mSHLight[i];
	}

	GLuint shLight;
	glGenBuffers(1, &shLight);
	glBindBuffer(GL_UNIFORM_BUFFER, shLight);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SHBlock), &block, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLRender::render(FrameBuffer* fb)
{

	//renderShadow();
	fb->bindForDraw();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 960, 720);
	renderBackground();
	renderObject();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::renderShadow()
{
	GLfloat near_plane = 1.f, far_plane = 10.f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 shadowView = glm::lookAt(mRenderScene->mScene->mLights[0]->getParallelLightDir(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	mRenderScene->mShadowmap->bindForDraw(0);
	mRenderScene->mShadowmap->mShadowProgram->use();
	mRenderScene->mShadowmap->mShadowProgram->setMat4f("V", shadowView);
	mRenderScene->mShadowmap->mShadowProgram->setMat4f("P", shadowProj);
	glCullFace(GL_FRONT);
	for (int i = 0; i < mRenderScene->mSceneMeshParam.size(); i++)
	{
		glm::mat4 model = mRenderScene->mSceneMeshParam[i].mTrans* mRenderScene->mSceneMeshParam[i].mRotate*mRenderScene->mSceneMeshParam[i].mScale;
		mRenderScene->mShadowmap->mShadowProgram->setMat4f("M", model);
		mRenderScene->mSceneMeshParam[i].mVAO->draw();
	}
	glCullFace(GL_BACK);
	mRenderScene->mShadowmap->mShadowProgram->release();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::renderObject()
{
	glm::mat4 view, proj;
	view = mRenderScene->mScene->mCamera->getViewMat();
	proj = mRenderScene->mScene->mCamera->getProjMat();

	glViewport(0, 0, 960, 720);
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < mRenderScene->mSceneMeshParam.size(); i++)
	{
		auto mtl = mRenderScene->mMaterials[mRenderScene->mSceneMeshParam[i].mMatIdx];
		mtl->use();
		glm::mat4 model = mRenderScene->mSceneMeshParam[i].mTrans* mRenderScene->mSceneMeshParam[i].mRotate*mRenderScene->mSceneMeshParam[i].mScale;
		mtl->setMat4f("M", model);
		mtl->setMat4f("V", view);
		mtl->setMat4f("P", proj);
		mtl->setVec3("lightDir", mRenderScene->mScene->mLights[0]->getParallelLightDir());
		mtl->setVec3("lightColor", mRenderScene->mScene->mLights[0]->getLightColor());
		mtl->setVec3("objectColor", glm::vec3(1, 1, 1));
		mtl->setInt("ShadowMap", 0);
		mtl->setBlock("SHLightCoeff", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mRenderScene->mShadowmap->getShadowTexture());
		mRenderScene->mSceneMeshParam[i].mVAO->draw();
		mtl->release();
	}
}

void GLRender::renderBackground()
{
	glm::mat4 view = glm::mat4(glm::mat3(mRenderScene->mScene->mCamera->getViewMat()));
	glm::mat4 proj = mRenderScene->mScene->mCamera->getProjMat();
	mRenderScene->mEnvmap->drawBackground(view, proj);
}