#pragma once


//math helpers
#include <math.h>

#ifndef PI
constexpr float PI = 3.14159265f;
#endif
constexpr double PI_D = 3.1415926535897932;

template <typename T>
constexpr auto sq(const T& x)
{
	return x * x;
}

template<typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}