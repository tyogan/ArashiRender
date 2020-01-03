#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "vao.h"
#include "shadowmap.h"
#include "envmap.h"
#include "camera.h"

struct Param
{
	glm::mat4 M;
	glm::mat4 V;
	glm::mat4 P;
};

class RENDER_API GLRender
{
public:
	GLRender();
	~GLRender();

	GLuint render();

	void renderBackground();
	void renderObject();
	void renderShadow();

	void renderGBuffer();

	GLuint getTexture();

private:
	void initShader();
	void initVAO();
	void initParams();

	ShaderProgram* mObjShader;
	ShaderProgram* mShadowShader;
	ShaderProgram* mBgShader;
	ShaderProgram* mCubeShader;

	Shadowmap mShadowmap;
	Envmap mEnvmap;
private:
	vector<VAO*> mVAOs;
	VAO* mBgVAO;

	GLuint mFBO;
	GLuint mTexture;

	Param mParam;
	Camera* mCamera;
};