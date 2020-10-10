#include "image.h"
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

template<>
bool Image<float>::save(const std::string& filename, bool flip) const
{
	stbi_flip_vertically_on_write(flip);
	return stbi_write_hdr(filename.c_str(), width, height, channels, (float*)_data.data());
}

template<>
bool Image<unsigned char>::save(const std::string& filename, bool flip) const
{
	stbi_flip_vertically_on_write(flip);
	return stbi_write_jpg(filename.c_str(), width, height, channels, _data.data(), 95);	
}

// load from file
template<>
bool Image<unsigned char>::load(const std::string& file, bool flip)
{
	stbi_set_flip_vertically_on_load(flip);

	int w, h, c;
	unsigned char* temp = stbi_load(file.c_str(), &w, &h, &c, STBI_default);

	if (temp == 0)
		return false;
	resize(w, h, c);
	memcpy(_data.data(), temp, w * h * c);
	stbi_image_free(temp);

	return true;
}

template<>
bool Image<float>::load(const std::string& file, bool flip)
{
	stbi_set_flip_vertically_on_load(flip);

	int w, h, c;

	// we do not want gamma here
	stbi_ldr_to_hdr_scale(1.0f);
	stbi_ldr_to_hdr_gamma(1.0f);

	float* temp = stbi_loadf(file.c_str(), &w, &h, &c, STBI_default);

	if (temp == 0)
		return false;

	resize(w, h, c);
	memcpy(_data.data(), temp, w * h * c * sizeof(float));
	stbi_image_free(temp);
	return true;
}

// load from memory buffer
template<>
bool Image<unsigned char>::load(const char* buffer, size_t len, bool flip)
{
	stbi_set_flip_vertically_on_load(flip);

	int w, h, c;
	unsigned char* temp = stbi_load_from_memory((unsigned char*)buffer, len, &w, &h, &c, STBI_default);

	if (temp)
	{
		resize(w, h, c);
		memcpy(_data.data(), temp, w * h * c);
		stbi_image_free(temp);
		return true;
	}
	// failed
	return false;
}

template<>
bool Image<float>::load(const char* buffer, size_t len, bool flip)
{
	stbi_set_flip_vertically_on_load(flip);

	int w, h, c;
	float* temp = stbi_loadf_from_memory((unsigned char*)buffer, len, &w, &h, &c, STBI_default);

	if (temp)
	{
		resize(w, h, c);
		memcpy(_data.data(), temp, w * h * c * sizeof(float));
		stbi_image_free(temp);
		return true;
	}
	// failed
	return false;
}

