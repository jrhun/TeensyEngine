#pragma once

#define SCREEN_WIDTH	64
#define SCREEN_HEIGHT	32

#include "Vec3.h"
#include <limits>

class Graphics {
public:
	struct mRGB {
		mRGB(int r, int g, int b) : r(r), g(g), b(b) {}
		mRGB(int x) : r((x >> 16) & 0xFF), g((x >> 8) & 0xFF), b((x >> 0) & 0xFF) {}
		union {
			unsigned char raw[3];
			struct {
				char r, g, b;
			};
		};

	};

	void init() {}
	void show() {}
	void clear() {}
	void fade(unsigned char a) {}

	virtual void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {};
	virtual void putPixel(int x, int y, mRGB c) {};
	virtual void putPixel(int x, int y, unsigned char h) {};

	void drawPointDepth(Vec3 &v, unsigned char r, unsigned char g, unsigned char b) {
		if (testAndSetZ(v.x, v.y, v.z)) {
			float z = 255 - v.z;
			r = r * z / 255;
			g = g * z / 255;
			b = b * z / 255;
			putPixel(v.x, v.y, r, g, b);
		}
	}

	void DrawLineDepth(Vec3& v0, Vec3& v1, unsigned char r, unsigned char g, unsigned char b)
	{
		float dx = v1.x - v0.x;
		float dy = v1.y - v0.y;

		if (dy == 0.0f && dx == 0.0f)
		{
		}
		else if (abs(dy) > abs(dx))
		{
			if (dy < 0.0f)
			{
				std::swap(v0, v1);
				dy = -dy;
			}

			const auto dv = (v1 - v0) / dy;
			for (auto v = v0; v.y < v1.y; v += dv)
			{
				const auto x = int(v.x);
				const auto y = int(v.y);
				if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
				{
					continue;
				}
				//if (testAndSetZ(x, y, v.z))
				//{
					drawPointDepth(v, r, g, b);
				//}
			}
		}
		else
		{
			if (dx < 0.0f)
			{
				std::swap(v0, v1);
				dx = -dx;
			}

			const auto dv = (v1 - v0) / dx;
			for (auto v = v0; v.x < v1.x; v += dv)
			{
				const auto x = int(v.x);
				const auto y = int(v.y);
				if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
				{
					continue;
				}
				//if (testAndSetZ(x, y, v.z))
				//{
					drawPointDepth(v, r, g, b);
				//}
			}
		}
	}

	void resetZ() {
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
			zBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}

	bool testAndSetZ(int x, int y, float depth) {
		if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
			float &val = zBuffer[x + y * SCREEN_WIDTH];
			if (depth < val) {
				val = depth;
				return true;
			}
		}
		return false;
	}

	float zBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
};