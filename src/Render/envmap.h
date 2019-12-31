#pragma once

#include <string>

#include <vector>
#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#include "vao.h"

class RENDER_API Envmap
{
public:
	Envmap();
	~Envmap();
	void load(std::string path);
	void load(const std::vector<std::string>& imgNames);
	void bindCubeTexture();
	void bindImageTexture();
	void bindCreateCubeTexture();
	void bindIrradianceTexture();

	void createCubemapTexture();

	void createIrradianceTexture();

private:
	GLuint mCreateEnvCubemap;
	GLuint mCubeTexture;
	GLuint mImageTexture;
	GLuint mIrradianceTexture;

	GLuint mRBO;

	VAO* mVao;
	GLuint mFBO;
	ShaderProgram* mCubeProgram;
	ShaderProgram* mIrrProgram;
};