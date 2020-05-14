#pragma once

class RENDER_API Samplefunc
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
};