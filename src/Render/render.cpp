#include "render.h"
#include <time.h>
void GLRender::init()
{
	shared_ptr<Mesh> m = Mesh::createPlane();
	mPlane = shared_ptr<VAO>(new VAO);
	mPlane->create(m);
}

void GLRender::setLightMatrix(RenderScene* renderScene)
{
	struct SHBlock
	{
		glm::vec4 data[16];
	} block0;

	for (int i = 0; i < renderScene->mEnvmap->mSHLight.size(); i++)
	{
		block0.data[i] = glm::vec4(renderScene->mEnvmap->mSHLight[i], 1.f);
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
		glm::mat4 lightPV[128];
	} mLightBlock;

	GLfloat nearPlane = 1.f, farPlane = 10.f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 shadowView;
	for (int i = 0; i < renderScene->mEnvmap->mEnvLights.size(); i++)
	{
		shadowView = glm::lookAt(renderScene->mEnvmap->mEnvLights[i].mDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mLightBlock.lightDir[i] = glm::vec4(renderScene->mEnvmap->mEnvLights[i].mDir, 0.f);
		mLightBlock.lightColor[i] = glm::vec4(renderScene->mEnvmap->mEnvLights[i].mColor, 1.f);
		glm::mat4 tmp = shadowProj * shadowView;
		mLightBlock.lightPV[i] = shadowProj * shadowView;
	}

	GLuint envLight;
	glGenBuffers(1, &envLight);
	glBindBuffer(GL_UNIFORM_BUFFER, envLight);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(EnvLightBlock), &mLightBlock, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, envLight);
}

void GLRender::render(FrameBuffer* fb, RenderScene* renderScene)
{
	renderMainLightShadow(renderScene);
	renderEnvLightShadow(renderScene);
	fb->bindForDrawGBuffer();
	renderGBuffer(renderScene);
	renderSSAO(fb, renderScene);
	fb->bindForDrawImage();
	renderEnvmap(renderScene);
	renderObject(renderScene,fb);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::renderGBuffer(RenderScene* renderScene)
{
	glm::mat4 view, proj;
	view = renderScene->mScene->mCamera->getViewMat();
	proj = renderScene->mScene->mCamera->getProjMat();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 960, 720);
	glEnable(GL_DEPTH_TEST);
	renderScene->mGBufferProgram->use();
	for (int i = 0; i < renderScene->mRenderMeshParam.size(); i++)
	{
		glm::mat4 model = renderScene->mRenderMeshParam[i].mTrans* renderScene->mRenderMeshParam[i].mRotate*renderScene->mRenderMeshParam[i].mScale;
		renderScene->mGBufferProgram->setMat4f("M", model);
		renderScene->mGBufferProgram->setMat4f("V", view);
		renderScene->mGBufferProgram->setMat4f("P", proj);
		renderScene->mGBufferProgram->setInt("MeshIdx", i);
		renderScene->mGBufferProgram->setInt("MtlIdx", renderScene->mRenderMeshParam[i].mMatIdx);
		renderScene->mRenderMeshParam[i].mVAO->draw();
	}
	renderScene->mGBufferProgram->release();
}

void GLRender::renderMainLightShadow(RenderScene* renderScene)
{
	GLfloat nearPlane = 1.f, farPlane = 10.f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	
	glCullFace(GL_FRONT);
	renderScene->mShadowmap->mShadowProgram->use();
	for (int idxLight = 0; idxLight < renderScene->mScene->mLights.size(); idxLight++)
	{
		renderScene->mShadowmap->mShadowProgram->setMat4f("P", shadowProj);
		glm::mat4 shadowView = glm::lookAt(renderScene->mScene->mLights[0]->mLightDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		renderScene->mShadowmap->mShadowProgram->setMat4f("V", shadowView);
		
		renderScene->mShadowmap->bindForDraw(idxLight);
		for (int i = 0; i < renderScene->mRenderMeshParam.size(); i++)
		{
			glm::mat4 model = renderScene->mRenderMeshParam[i].mTrans* renderScene->mRenderMeshParam[i].mRotate*renderScene->mRenderMeshParam[i].mScale;
			renderScene->mShadowmap->mShadowProgram->setMat4f("M", model);
			renderScene->mRenderMeshParam[i].mVAO->draw();
		}
	}
	renderScene->mShadowmap->mShadowProgram->release();
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::renderEnvLightShadow(RenderScene* renderScene)
{
	glCullFace(GL_FRONT);
	GLfloat nearPlane = 1.f, farPlane = 10.f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 shadowView = glm::lookAt(renderScene->mScene->mLights[0]->mLightDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	for (int k = 0; k < renderScene->mEnvmap->mEnvLights.size(); k++)
	{
		shadowView = glm::lookAt(renderScene->mEnvmap->mEnvLights[k].mDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		renderScene->mEnvShadowmap->mShadowProgram->setMat4f("V", shadowView);
		for (int i = 0; i < renderScene->mRenderMeshParam.size(); i++)
		{
			glm::mat4 model = renderScene->mRenderMeshParam[i].mTrans* renderScene->mRenderMeshParam[i].mRotate*renderScene->mRenderMeshParam[i].mScale;
			renderScene->mEnvShadowmap->mShadowProgram->setMat4f("M", model);
			renderScene->mRenderMeshParam[i].mVAO->draw();
		}
	}
	renderScene->mEnvShadowmap->mShadowProgram->release();
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::renderObject(RenderScene* renderScene,FrameBuffer* fb)
{
	glm::mat4 view, proj;
	view = renderScene->mScene->mCamera->getViewMat();
	proj = renderScene->mScene->mCamera->getProjMat();

	glViewport(0, 0, 960, 720);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < renderScene->mRenderMeshParam.size(); i++)
	{
		int idx = renderScene->mRenderMeshParam[i].mMatIdx;
		auto mtl = renderScene->mMaterialLibraries[idx];

		mtl->use();
		glm::mat4 model = renderScene->mRenderMeshParam[i].mTrans* renderScene->mRenderMeshParam[i].mRotate*renderScene->mRenderMeshParam[i].mScale;
		mtl->setMat4f("M", model);
		mtl->setMat4f("V", view);
		mtl->setMat4f("P", proj);
		mtl->setVec3("lightDir", renderScene->mScene->mLights[0]->mLightDir);
		mtl->setVec3("lightColor", renderScene->mScene->mLights[0]->mLightColor);
		mtl->setVec3("objectColor", glm::vec3(1.f));

		switch (idx)
		{
		case 0: break;
		case 1: 
		{
			mtl->setBlock("SHLightCoeff", 0);
		}; break;
		case 2:
		{
			mtl->setBlock("EnvmapLightBlock", 1);
			mtl->setInt("envmapLightNums", renderScene->mEnvmap->mEnvLights.size());
			mtl->setInt("EnvShadowMap", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, renderScene->mEnvShadowmap->getShadowTexture());
		}; break;
		case 3:
		{
			mtl->setInt("irradianceMap", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, renderScene->mEnvmap->mIrradianceCubeTex);
			mtl->setInt("ssaoMap", 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fb->mSSAO->mTextures[0]);
		}; break;
		default:
			break;
		}

		renderScene->mRenderMeshParam[i].mVAO->draw();

		mtl->release();
	}
}

void GLRender::renderSSAO(FrameBuffer* fb, RenderScene* renderScene)
{
	glViewport(0, 0, 960, 720);
	fb->mSSAO->bindForCreate();
	fb->mSSAO->mSSAOProgram->use();

	glm::mat4 view = renderScene->mScene->mCamera->getViewMat();
	glm::mat4 proj = renderScene->mScene->mCamera->getProjMat();
	fb->mSSAO->mSSAOProgram->setMat4f("projection",proj);
	fb->mSSAO->mSSAOProgram->setMat4f("view", view);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fb->position);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fb->normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fb->mSSAO->mNoiseTexture);

	mPlane->draw();
	fb->mSSAO->mSSAOProgram->release();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::renderEnvmap(RenderScene* renderScene)
{
	glm::mat4 view = glm::mat4(glm::mat3(renderScene->mScene->mCamera->getViewMat()));
	glm::mat4 proj = renderScene->mScene->mCamera->getProjMat();
	renderScene->mEnvmap->drawBackground(view, proj);
}