#pragma once

#include <glm/glm.hpp>
#include <time.h>

class UTILITY_API Samplefunc
{
public:
	static float halton(int idx, int base)
	{
		float f = 1.0f;
		float r = 0.f;
		while (idx > 0)
		{
			f = f / base;
			r = r + f * (idx%base);
			idx = idx / base;
		}
		return r;
	}
	static glm::vec2 * samplePoints(int num)
	{
		glm::vec2 *pnts = new glm::vec2[num];
		srand(time(nullptr));
		for (unsigned int i = 0; i < num; i++)
		{
			float uOffset = float(rand()) / RAND_MAX;
			float vOffset = float(rand()) / RAND_MAX;
			float u = halton(i, 2) + uOffset;
			float v = halton(i, 3) + vOffset;
			if (u >= 1) u -= 1.f;
			if (v >= 1) v -= 1.f;
			pnts[i].x = u;
			pnts[i].y = v;
		}
		return pnts;
	}

	static float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
};