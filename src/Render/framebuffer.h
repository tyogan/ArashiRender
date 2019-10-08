#include "glad/glad.h"

class RENDER_API Framebuffer
{
public:
	Framebuffer(GLuint width,GLuint height);
	~Framebuffer();
private:
	GLuint mDepthMapFBO;
};

Framebuffer::Framebuffer(GLuint width, GLuint height)
{

}