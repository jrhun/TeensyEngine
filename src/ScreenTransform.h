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
		if (v.z < 0) {
			v.x = 0;
			v.y = 0;
			v.z = 0;
			return v;
		}
		float invZ = 1.0f / v.z;
		v.x *= invZ;
		v.y *= invZ;
		// z goes from model space -1 to 4
		// screen space 1 to 6 centered around 2
		// z 2 = alpha of 255, set back cull to z > 6
		v.z = 255 - ((constrain(v.z, 1, 3) - 1) * 64);
		v.x = (v.x * height / 2) + (width / 2);
		v.y = (v.y * height / 2) + (height / 2);
		return v;
	}

	Vec3 & TransformSphere(Vec3 &v) {
		float L = v.Len();
		v /= L;
		float theta = atan2(v.z, v.x);
		if (theta > 0) {
			v.x = myMap(theta, 0, PI, width / 2, 0); // 0 exclusive, PI inclusive
		}
		else {
			v.x = myMap(theta, 0, -PI, width / 2, width); // 0 inclusive, -pi exclusive
		}
		v.z = 255 - ((constrain(L, 1, 3) - 1) * 64);
		v.y = (v.y * height / 2) + (height / 2);
		return v;

		//if (v.z >= 0)
		//	return TransformFrontP(v);
		//else if (v.z <= 0)
		//	return TransformBackP(v);
	}

	Vec3& TransformFront(Vec3 &v) {
		float invZ = 1.0f / v.z;
		v.x *= invZ;
		v.y *= invZ;
		// z goes from model space -1 to 4
		// screen space 1 to 6 centered around 2
		// z 2 = alpha of 255, set back cull to z > 6
		v.z = 255 - ((constrain(v.z, 2, 6) - 2) * zScale);
		v.x = (v.x * height / 2) + (height / 2);
		v.y = (v.y * height / 2) + (height / 2);
		//if (v.x >= width / 2 || v.x < 0)
		//	v.z = 0;
		return v;
	}

	Vec3& TransformBack(Vec3 &v) {
		//v.z = -v.z; //flip z for rendering back of sphere
		float invZ = 1.0f / v.z;
		v.x *= invZ;
		v.y *= invZ;
		// z goes from model space -1 to 4
		// screen space 1 to 6 centered around 2
		// z 2 = alpha of 255, set back cull to z > 6
		v.z = 255 - ((constrain(-v.z, 2, 6) - 2) * zScale);
		v.x = (v.x * height / 2) + (height / 2) + height;
		v.y = (v.y * height / 2) + (height / 2);
		//if (v.x < width / 2 || v.x > width)
		//	v.z = 0;
		return v;
	}


	Vec3 & TransformFrontP(Vec3 &v) {
		// intersection of point with parabola
		// parabola is at length 1 from origin
		// so divide by length
		float L = v.Len();
		v /= L;
		float theta = atan2(v.z, v.x);
		v.x = myMap(theta, PI, 0, 0, width / 2);
		v.z = 255 - ((constrain(L, 1, 3) - 1) * zScale * 2);
		//v.x = (v.x * height / 2) + (height / 2);
		v.y = (v.y * height / 2) + (height / 2);
		//if (v.x >= width / 2 || v.x < 0)
		//	v.z = 0;
		return (v);
	}

	Vec3 & TransformBackP(Vec3 &v) {
		// intersection of point with parabola
		// parabola is at length 1 from origin
		// so divide by length
		float L = v.Len();
		v /= L;
		float theta = atan2(v.z, v.x);
		v.x = myMap(theta, 0, -PI, 0, width / 2);
		v.z = 255 - ((constrain(L, 1, 3) - 1) * zScale * 2);
		v.x = (-v.x * height / 2) + (height / 2) + height;
		v.y = (v.y * height / 2) + (height / 2);
		//if (v.x < width / 2 || v.x > width)
			//v.z = 0;
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