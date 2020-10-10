#pragma once
#include "glad/glad.h"
#include "shader.h"

#include <memory>
using std::shared_ptr;

class RENDER_API Shadowmap
{
public:
	Shadowmap();
	~Shadowmap();
	void bindForDraw(int idx);
	GLuint getShadowTexture();

public:
	shared_ptr<ShaderProgram> mShadowProgram;

private:
	GLuint mFramebuffer;
	GLuint mTextures;
};