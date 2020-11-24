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
	void setRenderScene(RenderScene* renderScene);
	void render(FrameBuffer* fb);
	
private:
	void renderGBuffer();
	void renderShadow();
	void renderObject();
	void renderEnvmap();

private:
	RenderScene* mRenderScene;

	struct EnvLightBlock
	{
		glm::vec4 lightDir[128];
		glm::vec4 lightColor[128];
	} mLightBlock;

};