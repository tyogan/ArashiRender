#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"

#include "shader.h"

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

class RENDER_API Envmap
{
public:
	Envmap();
	~Envmap();
	void load(std::string path);
	void load(const std::vector<std::string>& imgNames);
	void bindImageTexture();
	void bindCubeTexture();
	void bindCreateCubeTexture();

	void createCubemapTexture();

	void createIrradianceTexture();
	//void extractLights();
private:
	void initFBO();

	GLuint mImageTexture;
	GLuint mCreateEnvCubemap;
	GLuint mCubeTexture;

	GLuint mIrradianceTexture;

	GLuint mRBO;

	GLuint mFBO;
	ShaderProgram* mCubeProgram;
};