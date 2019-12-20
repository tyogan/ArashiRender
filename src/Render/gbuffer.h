#include "glad/glad.h"

class RENDER_API GBuffer
{
public:
	GBuffer(GLuint width,GLuint height);
	~GBuffer();

	void bindForDraw();
private:
	GLuint mFBO;
	GLuint mTextures[3];
};