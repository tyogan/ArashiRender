#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"
#include "shadowmap.h"
#include "envmap.h"

#include "ibltree.h"

class RENDER_API GLRender
{
public:
	GLRender();
	~GLRender();

	GLuint render();

	void renderBackground();
	void renderObject();
	void renderShadow();

	GLuint getTexture();

private:
	void initFBO();

	Shadowmap mShadowmap;
	Envmap mEnvmap;
private:
	Scene mScene;

	GLuint mFBO;
	GLuint mTexture;
};