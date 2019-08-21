#pragma once


//math helpers
#include <math.h>

#ifndef PI
const float PI = 3.14159265f;
#endif
//const double PI_D = 3.1415926535897932;

template <typename T>
T sqvec(const T& x)
{
	return x * x;
}

template<typename T>
T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}

#if defined(ARDUINO)

#else
#include "ofMath.h"
//#include "math.h"

inline uint8_t constrain(int x, int min, int max) {
	return (int)ofClamp(x, min, max);
}

inline float myMap(float x, float inMin, float inMax, float outMin, float outMax) {
	return ofMap(x, inMin, inMax, outMin, outMax);
}

#endif