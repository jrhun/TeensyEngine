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
#define FLT_EPSILON      1.192092896e-07F   


float myMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false) {

	if (fabs(inputMin - inputMax) < FLT_EPSILON) {
		return outputMin;
	}
	else {
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

		if (clamp) {
			if (outputMax < outputMin) {
				if (outVal < outputMax)outVal = outputMax;
				else if (outVal > outputMin)outVal = outputMin;
			}
			else {
				if (outVal > outputMax)outVal = outputMax;
				else if (outVal < outputMin)outVal = outputMin;
			}
		}
		return outVal;
	}

}
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