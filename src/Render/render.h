#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.h"

class RENDER_API GLRender
{
public:
	GLRender();
	~GLRender();

	void render();

	void renderBackground();
	void renderObject();
	void renderShadow();

private:
	ShaderProgram* mObjShader;
	ShaderProgram* mShadowShader;
	ShaderProgram* mBgShader;
};