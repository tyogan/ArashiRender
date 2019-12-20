#pragma once

#include <string>

#include <vector>
#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"
class RENDER_API Envmap
{
public:
	Envmap();
	~Envmap();
	void load(std::string path);
	void bindForRead();
private:
	GLuint mTexture;
};