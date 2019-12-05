#include "glad/glad.h"

class RENDER_API Shadowmap
{
public:
	Shadowmap();
	~Shadowmap();
	void bindForDraw();
	GLuint readTexture();
private:
	GLuint mFramebuffer;
	GLuint mTexture;
};