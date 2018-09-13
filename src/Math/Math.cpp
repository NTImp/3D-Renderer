#include "Math.hpp"

#include <math.h>

#define RADIANS(a) ((float) a * M_PI / 180)


namespace Math {
	float Cos(float degrees)
	{
		return cos(RADIANS(degrees));
	}

	float Sin(float degrees)
	{
		return sin(RADIANS(degrees));
	}

	float Tan(float degrees)
	{
		return tan(RADIANS(degrees));
	}

	float Sqrt(float val)
	{
		return sqrt(val);
	}
}
