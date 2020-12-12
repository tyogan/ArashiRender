#pragma once

#include <sstream>>
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
	void renderObject(RenderScene* renderScene, FrameBuffer* fb);
	void renderEnvmap(RenderScene* renderScene);
	void renderEnvLightShadow(RenderScene* renderScene);

	void renderSSAO(FrameBuffer* fb, RenderScene* renderScene);

	shared_ptr<VAO> mPlane;
};