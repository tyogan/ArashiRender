#pragma once

#include <vector>
#include "stb_image.h"

template<typename T>
class Image
{	
public:
	Image()
	{
		width = 0;
		height = 0;
		channels = 0;
	}

	Image(const Image<T>& src)
	{
		width = src.width;
		height = src.height;
		channels = src.channels;
		_data = src._data;
	}

	Image(unsigned int w, unsigned int h, unsigned int c = 1)
	{
		resize(w, h, c);
	}

	void resize(unsigned int w, unsigned int h, unsigned int c)
	{
		width = w;
		height = h;
		channels = c;
		_data.resize(w * h * c);
	}
	
	void resize(unsigned int w, unsigned int h)
	{
		width = w;
		height = h;
		_data.resize(w * h * channels);
	}

	bool isValid() const
	{
		return width > 0 && height > 0 && channels > 0;
	}
	
	T& operator()(unsigned int x, unsigned int y, unsigned int c = 0)
	{
		return _data[(x + y * width) * channels + c];
	}

	const T& operator()(unsigned int x, unsigned int y, unsigned int c = 0) const
	{
		return _data[(x + y * width) * channels + c];
	}

	T*	data() { return _data.data(); }
	const T* data() const { return _data.data(); }

public:
	unsigned int width;
	unsigned int height;
	unsigned int channels;

public:
	bool save(const std::string& file, bool flip = true) const;
	bool load(const std::string& file, bool flip = true);
	bool load(const char* buffer, size_t len, bool flip = true);

private:
	std::vector<T> _data;
};