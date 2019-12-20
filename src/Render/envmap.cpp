#include "envmap.h"

using std::vector;
using std::string;

Envmap::Envmap()
{
	vector<string> imgNames
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
	int width, height, nrChannels;
	for (int i = 0; i < imgNames.size(); i++)
	{
		unsigned char* data = stbi_load(("bin/envmap/" + imgNames[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << imgNames[i] << std::endl;
			stbi_image_free(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

Envmap::~Envmap()
{

}

void Envmap::bindForRead()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
}

void Envmap::load(std::string path)
{

}