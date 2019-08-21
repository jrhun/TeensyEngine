#pragma once

#include "Vec3.h"
#include "mathHelpers.h"

class ScreenTransform {
public: 
	ScreenTransform() {}
	ScreenTransform(float width, float height) :width(width), height(height) {}

	template<class Vertex>
	Vertex& Transform(Vertex &v) {
		return Transform(v.pos);
		//float invZ = 1.0f / v.pos.z;
		//v.pos.x *= invZ;
		//v.pos.y *= invZ;
		//v.pos.x = (v.pos.x * height) + width/2;
		//v.pos.y = (v.pos.y * height) + height/2;
		//return v;
	}

	Vec3& Transform(Vec3 &v) {
		float invZ = 1.0f / v.z;
		v.x *= invZ;
		v.y *= invZ;
		// z goes from model space -1 to 4
		// screen space 1 to 6 centered around 2
		// z 2 = alpha of 255, set back cull to z > 6
		v.z = 255 - ((constrain(v.z, 2, 6) - 2) * zScale);
		v.x = (v.x * height) + (width / 2);
		v.y = (v.y * height) + (height / 2);
		return v;
	}

	Vec3 & TransformParabola(Vec3 &v) {
		// intersection of point with parabola
		// parabola is at length 1 from origin
		// so divide by length
		float L = v.Len();
		float z = v.z;
		v /= L; 
		if (v.z > 0)
			v.z += 1; // forward hemisphere
		else
			v.z -= 1;
		v.x /= v.z;
		v.y /= v.z;
		v.z = (L - 0.1) / (500 - 0.1);
		v.z = 255 - ((constrain(z, 2, 6) - 2) * zScale);
		v.x = (v.x * height) + (width / 2);
		v.y = (v.y * height) + (height / 2);
		return (v);
	}

	//Vec3 & TransformParabola(Vec3 &v) {
	//	float bx = v.z / v.x;
	//	float by = v.z / v.y;
	//	if (v.x >= 0) {
	//		v.x = 0.5f + v.x * sqrt(bx*bx + 4) / (2 * v.z);
	//	}
	//	else {
	//		v.x = - 0.5f - v.x * sqrt(bx*bx + 4) / (2 * v.z);
	//	}
	//	if (v.y >= 0) {
	//		v.y = 0.5f + v.y * sqrt(by*by + 4) / (2 * v.z);
	//	}
	//	else {
	//		v.y = -0.5f - v.y * sqrt(by*by + 4) / (2 * v.z);
	//	}
	//	v.z = 255 - ((constrain(v.z, 2, 6) - 2) * zScale);
	//	v.x = (v.x * height) + (width / 2);
	//	v.y = (v.y * height) + (height / 2);
	//	return v;
	//}

	float width;
	float height;
	float zScale = 32;
};