#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderscene.h"
#include "framebuffer.h"

class RENDER_API GLRender    
{
public:
	void render(FrameBuffer* fb, RenderScene* mRenderScene);

private:
	void renderShadow(RenderScene* mRenderScene);
	void renderObject(RenderScene* mRenderScene);
	void renderBackground(RenderScene* mRenderScene);
};