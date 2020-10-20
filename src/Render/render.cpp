#include "render.h"
#include <time.h>

GLRender::GLRender(RenderScene* renderScene)
	:mRenderScene(renderScene)
{
	struct SHBlock
	{
		glm::vec4 data[16];
	} block0;

	for (int i = 0; i < mRenderScene->mEnvmap->mSHLight.size(); i++)
	{
		block0.data[i] = glm::vec4(mRenderScene->mEnvmap->mSHLight[i], 1.f);
	}

	GLuint shLight;
	glGenBuffers(1, &shLight);
	glBindBuffer(GL_UNIFORM_BUFFER, shLight);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SHBlock), &block0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, shLight);

	struct EnvLightBlock
	{
		glm::vec4 lightDir[128];
		glm::vec4 lightColor[128];
	} block1;

	float u, v;

	for (int i = 0; i < 128; i++)
	{
		u = mRenderScene->mEnvmap->mBgSample[i].pos.x;
		v = mRenderScene->mEnvmap->mBgSample[i].pos.y;
		double phi = 2 * 3.1415926 * u;
		double theta = acos(1 - 2 * v);
		double r = sin(theta);
		block1.lightDir[i] = -glm::vec4(r * cos(phi), cos(theta), r * sin(phi), 0.f);
		block1.lightColor[i] = glm::vec4(mRenderScene->mEnvmap->mBgSample[i].color, 1.f);
	}

	GLuint envLight;
	glGenBuffers(1, &envLight);
	glBindBuffer(GL_UNIFORM_BUFFER, envLight);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(EnvLightBlock), &block1, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, envLight);
}

void GLRender::render(FrameBuffer* fb)
{
	//renderShadow();
	fb->bindForDrawGBuffer();
	renderGBuffer();
	fb->bindForDrawImage();
	renderBackground();
	renderObject();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::renderGBuffer()
{
	glm::mat4 view, proj;
	view = mRenderScene->mScene->mCamera->getViewMat();
	proj = mRenderScene->mScene->mCamera->getProjMat();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 960, 720);
	glEnable(GL_DEPTH_TEST);
	mRenderScene->mGBufferProgram->use();
	for (int i = 0; i < mRenderScene->mRenderMeshParam.size(); i++)
	{
		glm::mat4 model = mRenderScene->mRenderMeshParam[i].mTrans* mRenderScene->mRenderMeshParam[i].mRotate*mRenderScene->mRenderMeshParam[i].mScale;
		mRenderScene->mGBufferProgram->setMat4f("M", model);
		mRenderScene->mGBufferProgram->setMat4f("V", view);
		mRenderScene->mGBufferProgram->setMat4f("P", proj);
		mRenderScene->mGBufferProgram->setInt("MeshIdx", i);
		mRenderScene->mGBufferProgram->setInt("MtlIdx", mRenderScene->mRenderMeshParam[i].mMatIdx);
		mRenderScene->mRenderMeshParam[i].mVAO->draw();
	}
	mRenderScene->mGBufferProgram->release();
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
	for (int i = 0; i < mRenderScene->mRenderMeshParam.size(); i++)
	{
		glm::mat4 model = mRenderScene->mRenderMeshParam[i].mTrans* mRenderScene->mRenderMeshParam[i].mRotate*mRenderScene->mRenderMeshParam[i].mScale;
		mRenderScene->mShadowmap->mShadowProgram->setMat4f("M", model);
		mRenderScene->mRenderMeshParam[i].mVAO->draw();
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
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < mRenderScene->mRenderMeshParam.size(); i++)
	{
		auto mtl = mRenderScene->mMaterials[mRenderScene->mRenderMeshParam[i].mMatIdx];
		mtl->use();
		glm::mat4 model = mRenderScene->mRenderMeshParam[i].mTrans* mRenderScene->mRenderMeshParam[i].mRotate*mRenderScene->mRenderMeshParam[i].mScale;
		mtl->setMat4f("M", model);
		mtl->setMat4f("V", view);
		mtl->setMat4f("P", proj);
		mtl->setVec3("lightDir", mRenderScene->mScene->mLights[0]->getParallelLightDir());
		mtl->setVec3("lightColor", mRenderScene->mScene->mLights[0]->getLightColor());
		mtl->setVec3("objectColor", glm::vec3(1.f));
		mtl->setInt("ShadowMap", 0);
		mtl->setBlock("SHLightCoeff", 0);
		mtl->setBlock("EnvmapLightBlock", 1);
		mtl->setVec3("coeff", mRenderScene->mEnvmap->mSHLight[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mRenderScene->mShadowmap->getShadowTexture());
		mRenderScene->mRenderMeshParam[i].mVAO->draw();
		mtl->release();
	}
}

void GLRender::renderBackground()
{
	glm::mat4 view = glm::mat4(glm::mat3(mRenderScene->mScene->mCamera->getViewMat()));
	glm::mat4 proj = mRenderScene->mScene->mCamera->getProjMat();
	mRenderScene->mEnvmap->drawBackground(view, proj);
}