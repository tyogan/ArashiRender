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
#include "object.h"

#include "vao.h"

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

private:
	void initFBO();

	GLuint mImageTexture;
	GLuint mCreateEnvCubemap;
	GLuint mCubeTexture;

	GLuint mIrradianceTexture;

	GLuint mRBO;

	Object* mObject;
	GLuint mFBO;
	ShaderProgram* mCubeProgram;
	ShaderProgram* mIrrProgram;
};