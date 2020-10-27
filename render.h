#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\scene.h"
#include "shadowmap.h"
#include "renderscene.h"

#include "Utility\samplefunc.h"
#include "ibltree.h"
#include "iblsample.h"



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

	vector<Light> sampleImage1(int lightnums, string path);
	vector<Light> sampleImage2(int lightnums, string path);
	void extractLights(int nums);

	Shadowmap mShadowmap;
	Envmap mEnvmap;
private:
	Scene mScene;

	GLuint mFBO;
	GLuint mTexture;
};