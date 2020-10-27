#include "render.h"

#include <time.h>

GLRender::GLRender()
{
	initFBO();
	mEnvmap.load("bin/envmap/alex.hdr");
	
	glm::mat4 model;
	//model = glm::translate(model, glm::vec3(0, 1.f, 0));
	model = glm::scale(model, glm::vec3(1.5f));
	//model = glm::rotate(model, 35.f, glm::vec3(0, 1.f, 0.f));
	mScene.loadObject("bin/model/teapot.obj", model);
	model = glm::scale(model, glm::vec3(3.5f,0.01f,3.5f));
	mScene.createCube(model);
	
	extractLights(63);

}

GLRender::~GLRender()
{
}

GLuint GLRender::getTexture()
{
	//return mShadowmap.getTexture();
	return mTexture;
}

GLuint GLRender::render()
{
	renderShadow();
	mEnvmap.createCubemapTexture();
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 960, 720);
	renderBackground();
	renderObject();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 0;
}

void GLRender::renderBackground()
{
	glm::mat4 view = glm::mat4(glm::mat3(mScene.getCamera(0).getViewMat()));
	glm::mat4 proj = mScene.getCamera(0).getProjMat(60.f, 960.f / 720.f, 0.1f, 1000.f);
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
	proj = mScene.getCamera(0).getProjMat(60.f, 960.f / 720.f, 0.1f, 1000.f);

	glViewport(0, 0, 960, 720);
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mShadowmap.getTexture());

	mScene.getShader(2)->use();
	mScene.getShader(2)->setMat4f("V", view);
	mScene.getShader(2)->setMat4f("P", proj);
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
	GLfloat near_plane = 1.f, far_plane = 10.f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 shadowView;

	for (int idx = 0; idx < 64; idx++)
	{
		shadowView = glm::lookAt(mScene.getLight(idx).getParallelLightDir(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mShadowmap.bindForDraw(idx);
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

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::initFBO()
{
	glGenFramebuffers(1, &mFBO);

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 720); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRender::extractLights(int nums)
{
	vector<Light> lights = sampleImage1(nums, "bin/envmap/alex.hdr");
	
	vector<glm::vec4> lightDir;
	vector<glm::vec4> lightColor;
	vector<glm::mat4> lightTrans;

	GLfloat near_plane = 1.0f, far_plane = 10.f;
	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 shadowView = glm::mat4(1); 

	lightDir.push_back(glm::vec4(mScene.getLight(0).getParallelLightDir(), 0));
	lightColor.push_back(glm::vec4(mScene.getLight(0).getLightColor(), 0));
	shadowView = glm::lookAt(mScene.getLight(0).getParallelLightDir(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightTrans.push_back(shadowProj*shadowView);

	for (int i = 0; i < nums; i++)
	{
		mScene.addLight(lights[i]);
		lightDir.push_back(glm::vec4(lights[i].getPointLightPos(),0));
		lightColor.push_back(glm::vec4(lights[i].getLightColor(), 0));
		shadowView = glm::lookAt(lights[i].getPointLightPos(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightTrans.push_back(shadowProj*shadowView);
	}

	unsigned int lightBlock;
	glGenBuffers(1, &lightBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock);
	glBufferData(GL_UNIFORM_BUFFER, 128 * sizeof(glm::vec4)+64*sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 2, lightBlock);
	mScene.getShader(2)->setBlock("lightBlock", 2);
	
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64*sizeof(glm::vec4), lightDir.data());
	glBufferSubData(GL_UNIFORM_BUFFER, 64*sizeof(glm::vec4), 64*sizeof(glm::vec4), lightColor.data());
	glBufferSubData(GL_UNIFORM_BUFFER, 128 * sizeof(glm::vec4), 64 * sizeof(glm::mat4), lightTrans.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

vector<Light> GLRender::sampleImage1(int lightnums,string path)
{
	vector<Light> lights;

	glm::vec2 *pnts = new glm::vec2[lightnums];
	srand(time(nullptr));
	for (unsigned int i = 0; i < lightnums; i++)
	{
		float u_offset = float(rand()) / RAND_MAX;
		float v_offset = float(rand()) / RAND_MAX;
		float u = Samplefunc::halton(i, 2) + u_offset;
		float v = Samplefunc::halton(i, 3) + v_offset;
		if (u >= 1) u -= 1.f;
		if (v >= 1) v -= 1.f;
		pnts[i].x = u;
		pnts[i].y = v;
	}
	Image<float> img;
	img.load(path.c_str());
	IBLTree tree(16, img);
	CSample* sample = tree.sampleWraping(lightnums, pnts);
	
	for (unsigned int idx = 0; idx < lightnums; idx++)
	{
		float u1 = sample[idx].pos.x;
		float v1 = sample[idx].pos.y;
		int u = u1*img.width;
		int v = v1* img.height;

		for (int i =u - 3; i <= u + 3; i++)
		{
			for (int j = v - 3; j <= v + 3; j++)
			{
				if (i >= 0 && i<img.width && j>=0 && j<img.height)
				{
					img(i, j, 0) = 1.0f;
					img(i, j, 1) = 0;
					img(i, j, 2) = 0;
				}
			}
		}
		double phi = 2 * 3.1415926 * u1;
		double theta = acos(1 - 2 * v1);
		double r = sin(theta);
		glm::vec3 dir = -glm::vec3(r * cos(phi), cos(theta), r * sin(phi));
		lights.push_back(Light(dir, sample[idx].color));
	}

	img.save("bin/envmap/sample11.hdr");
	return lights;
}

vector<Light> GLRender::sampleImage2(int lightnums,string path)
{
	vector<Light> lights;
	glm::vec2 *pnts = new glm::vec2[lightnums];
	for (unsigned int i = 0; i < lightnums; i++)
	{
		float u_offset = float(rand()) / RAND_MAX;
		float v_offset = float(rand()) / RAND_MAX;
		float u = Samplefunc::halton(i, 2) + u_offset;
		float v = Samplefunc::halton(i, 3) + v_offset;
		if (u >= 1) u -= 1.f;
		if (v >= 1) v -= 1.f;
		pnts[i].x = u;
		pnts[i].y = v;
	}
	Image<float> img;
	img.load(path.c_str());
	IBLSample iblsample(img);

	Sample* sample = iblsample.sampleImage(lightnums, pnts);

	for (unsigned int idx = 0; idx < lightnums; idx++)
	{
		int u = sample[idx].mPos.x;
		int v = sample[idx].mPos.y;

		for (int i = u - 3; i <= u + 3; i++)
		{
			for (int j = v - 3; j <= v + 3; j++)
			{
				if (i >= 0 && i<img.width && j >= 0 && j<img.height)
				{
					img(i, j, 0) = 1.0f;
					img(i, j, 1) = 0;
					img(i, j, 2) = 0;
				}
			}
		}
		double u1 = (double)u / img.width;
		double v1 = (double)v / img.height;
		double phi = 2 * 3.1415926 * u1;
		double theta = acos(1 - 2 * v1);
		double r = sin(theta);

		lights.push_back(Light(-glm::vec3(r * cos(phi), cos(theta), r * sin(phi)), sample[idx].mColor));
	}

	img.save("bin/envmap/sample22.hdr");
	return lights;
}



