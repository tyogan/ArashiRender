//#include "render.h"
//#include "importer.h"
//#include "shadowmap.h"
//#include "camera.h"
//
//GLRender::GLRender()
//{
//	glGenFramebuffers(1, &mFBO);
//	glGenTextures(1, &mTexture);
//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTexture);
//
//	initShader();
//	initVAO();
//}
//GLRender::~GLRender()
//{
//	delete mObjShader;
//	delete mShadowShader;
//	delete mBgShader;
//}
//
//GLuint GLRender::render()
//{
//	Camera cam(glm::vec3(0, 0, 10.f), glm::vec3(0, 0, 0), glm::vec3(0, 1.f, 0));
//	glm::mat4 model, view, proj;
//	//model = glm::translate(model, glm::vec3(0, -3.f, 0));
//	model = glm::scale(model, glm::vec3(0.2f));
//	model = glm::rotate(model, 130.f, glm::vec3(0, 1.f, 0.f));
//	view = cam.getViewMat();
//	proj = cam.getProjMat(45.f, 800.f / 600.f, 0.1f, 100.f);
//
//	GLfloat near_plane = 1.0f, far_plane = 7.5f;
//	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//	glm::mat4 shadowView = glm::lookAt(glm::vec3(2.0f, 4.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//	glViewport(0, 0, 800, 600);
//	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, mShadowmap.readTexture());
//	mObjShader->use();
//	mObjShader->setMat4f("M", model);
//	mObjShader->setMat4f("V", view);
//	mObjShader->setMat4f("P", proj);
//	mObjShader->setMat4f("lightV", shadowView);
//	mObjShader->setMat4f("lightP", shadowProj);
//	mObjShader->setVec3("lightDir", glm::vec3(2.0f, 4.0f, 1.0f));
//	mObjShader->setVec3("viewPos", cam.getPos());
//	for (int i = 0; i < mVAOs.size(); i++)
//	{
//		mVAOs[i]->draw();
//	}
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	return 0;
//}
//
//void GLRender::renderShadow()
//{;
//	glm::mat4 shadowModel = glm::scale(shadowModel, glm::vec3(0.2f));
//	GLfloat near_plane = 1.0f, far_plane = 7.5f;
//	glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//	glm::mat4 shadowView = glm::lookAt(glm::vec3(2.0f, 4.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//	
//	glViewport(0, 0, 1024, 1024);
//	mShadowmap.bindForDraw();
//	mShadowShader->use();
//	mShadowShader->setMat4f("M", shadowModel);
//	mShadowShader->setMat4f("V", shadowView);
//	mShadowShader->setMat4f("P", shadowProj);
//	glCullFace(GL_FRONT);
//	for (int i = 0; i < mVAOs.size(); i++)
//	{
//		mVAOs[i]->draw();
//	}
//
//	glCullFace(GL_BACK);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void GLRender::initShader()
//{
//	mObjShader = new ShaderProgram("bin/shader/obj_vert.glsl", "bin/shader/obj_frag.glsl");
//	mObjShader->use();
//	mObjShader->setInt("container", 0);
//	mObjShader->setInt("shadowmap", 1);
//	mShadowShader = new ShaderProgram("bin/shader/shadowmap_vert.glsl", "bin/shader/shadowmap_frag.glsl");
//	mBgShader = new ShaderProgram("bin/shader/envmap_vert.glsl", "bin/shader/envmap_frag.glsl");
//}
//
//void GLRender::initVAO()
//{
//	for (int i = 0; i < mVAOs.size(); i++)
//	{
//		delete mVAOs[i];
//	}
//
//	Mesh cubeMesh = Mesh::createCube();
//	VAO* cubeVAO = new VAO();
//	cubeVAO->create(cubeMesh);
//	mVAOs.push_back(cubeVAO);
//
//	Mesh planeMesh = Mesh::createPlane();
//	VAO* planeVAO = new VAO();
//	planeVAO->create(planeMesh);
//	mVAOs.push_back(planeVAO);
//
//	vector<Mesh> bunnyMeshes;
//	Importer im;
//	im.loadModel("bin/model/Scoobydoo.obj", bunnyMeshes);
//
//	VAO* tmpVAO;
//	for (unsigned int i = 0; i < bunnyMeshes.size(); i++)
//	{
//		tmpVAO = new VAO();
//		tmpVAO->create(bunnyMeshes[i]);
//		mVAOs.push_back(tmpVAO);
//	}
//}
//
//void tmFunc()
//{
//	GLuint fbo, texture;
//	glGenFramebuffers(1, &fbo);
//	glGenTextures(1, &texture);
//	
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
//
//}