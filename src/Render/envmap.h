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
	void load(const std::vector<std::string>& imgNames);
	void bindCubeTexture();
	void bindTexture();

private:
	GLuint mCubeTexture;
	GLuint mTexture;
};