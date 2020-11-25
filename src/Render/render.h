#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderscene.h"
#include "framebuffer.h"

class RENDER_API GLRender    
{
public:
	void init();
	void render(FrameBuffer* fb, RenderScene* renderScene);
	
private:
	void renderGBuffer(RenderScene* renderScene);
	void renderShadow(RenderScene* renderScene);
	void renderObject(RenderScene* renderScene);
	void renderEnvmap(RenderScene* renderScene);

private:
	struct EnvLightBlock
	{
		glm::vec4 lightDir[128];
		glm::vec4 lightColor[128];
	} mLightBlock;
};