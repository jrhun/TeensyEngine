#pragma once

#include "Vec3.h"

class ScreenTransform {
public: 
	ScreenTransform() {}
	ScreenTransform(float width, float height) :width(width), height(height) {}

	template<class Vertex>
	Vertex& Transform(Vertex &v) {
		float invZ = 1.0f / v.pos.z;
		v.pos.x *= invZ;
		v.pos.y *= invZ;
		v.pos.x = (v.pos.x * height) + width/2;
		v.pos.y = (v.pos.y * height) + height/2;
		return v;
	}

	float width;
	float height;
};