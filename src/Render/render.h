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

struct Param
{
	glm::vec3 M;
	glm::vec3 V;
	glm::vec3 P;
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

	ShaderProgram* mObjShader;
	ShaderProgram* mShadowShader;
	ShaderProgram* mBgShader;

	Shadowmap mShadowmap;
	Envmap mEnvmap;
private:
	vector<VAO*> mVAOs;
	VAO* mBgVAO;

	GLuint mFBO;
	GLuint mTexture;
};