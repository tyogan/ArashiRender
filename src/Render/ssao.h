#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <random>
#include <string>

#include "shader.h"
#include "Utility/samplefunc.h"

using std::vector;

class RENDER_API SSAO
{
public:
	SSAO(GLuint width, GLuint height);
	~SSAO();

	void bindForCreate();

	GLuint getTexture();
	GLuint getBlurTexture();

private:
	void sampleKernel();

public:
	GLuint mFBO;
	union
	{
		GLuint mTextures[2];
		struct {
			GLuint ssaoTex, ssaoBlurTex;
		};
	};

	GLuint mWidth;
	GLuint mHeight;
	vector<glm::vec3> mKernel;
	GLuint mNoiseTexture;

	ShaderProgram* mSSAOProgram;
	ShaderProgram* mSSAOBlurProgram;
};