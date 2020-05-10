#pragma once
#include "glad/glad.h"

class RENDER_API Shadowmap
{
public:
	Shadowmap();
	~Shadowmap();
	void bindForDraw();
	GLuint getTexture();
private:
	GLuint mFramebuffer;
	GLuint mTexture;
};