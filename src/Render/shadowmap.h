#pragma once
#include "glad/glad.h"

class RENDER_API Shadowmap
{
public:
	Shadowmap();
	~Shadowmap();
	void bindForDraw(GLuint idx);
	GLuint getTexture();
private:
	GLuint mFramebuffer;
	GLuint mTextures;
};