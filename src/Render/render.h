#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderscene.h"
#include "framebuffer.h"

struct RenderParam
{

	bool isUseShadow;
	int mShadowNums;
	int mShadowSize;
};

class RENDER_API GLRender    
{
public:
	void init();
	void render(FrameBuffer* fb, RenderScene* renderScene);
	
	void setLightMatrix(RenderScene* renderScene);

private:
	void renderGBuffer(RenderScene* renderScene);
	void renderMainLightShadow(RenderScene* renderScene);
	void renderObject(RenderScene* renderScene);
	void renderEnvmap(RenderScene* renderScene);
	void renderEnvmapShadow(RenderScene* renderScene);

private:
	struct EnvLightBlock
	{
		glm::vec4 lightDir[128];
		glm::vec4 lightColor[128];
	} mLightBlock;
};