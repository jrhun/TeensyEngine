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
			float z = v.z;
			r = r * z / 255;
			g = g * z / 255;
			b = b * z / 255;
			putPixel(v.x, v.y, r, g, b);
		}
	}

	void drawPointDepth(int16_t x, int16_t y, int16_t z, unsigned char r, unsigned char g, unsigned char b) {
		if (testAndSetZ(x, y, z)) {
			r = r * z / 255;
			g = g * z / 255;
			b = b * z / 255;
			putPixel(x, y, r, g, b);
		}
	}

	void drawLineDepth(Vec3 &v0, Vec3 &v1, unsigned char r, unsigned char g, unsigned char b) {
		int16_t x0 = v0.x;
		int16_t y0 = v0.y;
		int16_t z0 = v0.z;
		int16_t x1 = v1.x;
		int16_t y1 = v1.y;
		int16_t z1 = v1.z;

		int16_t dx = abs(x1 - x0);
		int16_t sx = x0 < x1 ? 1 : -1;
		int16_t dy = abs(y1 - y0);
		int16_t sy = y0 < y1 ? 1 : -1;
		int16_t dz = abs(z1 - z0);
		int16_t sz = z0 < z1 ? 1 : -1;
		int16_t dm = max(max(dx, dy), dz);
		int16_t i = dm; /* maximum difference */
		x1 = dm / 2;
		y1 = dm / 2;
		z1 = dm / 2; /* error offset */

		for (;;) { /* loop */
			drawPointDepth(x0,y0,z0, r, g, b);

			if (i-- == 0) break;
			x1 -= dx;
			if (x1 < 0) {
				x1 += dm;
				x0 += sx;
			}
			y1 -= dy;
			if (y1 < 0) {
				y1 += dm;
				y0 += sy;
			}
			z1 -= dz;
			if (z1 < 0) {
				z1 += dm;
				z0 += sz;
			}
		}
	}

	void drawTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, Vec3 &c) {
		const Vec3* pv0 = &v0;
		const Vec3* pv1 = &v1;
		const Vec3* pv2 = &v2;

		// sorting vertices by y
		if (pv1->y < pv0->y) std::swap(pv0, pv1);
		if (pv2->y < pv1->y) std::swap(pv1, pv2);
		if (pv1->y < pv0->y) std::swap(pv0, pv1);

		if (pv0->y == pv1->y) // natural flat top
		{
			// sorting top vertices by x
			if (pv1->x < pv0->x) std::swap(pv0, pv1);
			drawFlatTopTriangle(*pv0, *pv1, *pv2, c);
		}
		else if (pv1->y == pv2->y) // natural flat bottom
		{
			// sorting bottom vertices by x
			if (pv2->x < pv1->x) std::swap(pv1, pv2);
			drawFlatBottomTriangle(*pv0, *pv1, *pv2, c);
		}
		else // general triangle
		{
			// find splitting vertex
			const float alphaSplit =
				(pv1->y - pv0->y) /
				(pv2->y - pv0->y);
			const Vec3 vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

			if (pv1->x < vi.x) // major right
			{
				drawFlatBottomTriangle(*pv0, *pv1, vi, c);
				drawFlatTopTriangle(*pv1, vi, *pv2, c);
			}
			else // major left
			{
				drawFlatBottomTriangle(*pv0, vi, *pv1, c);
				drawFlatTopTriangle(vi, *pv1, *pv2, c);
			}
		}
	}

	void drawFlatTopTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, Vec3 &c)
	{
		// calulcate slopes in screen space
		float m0 = (v2.x - v0.x) / (v2.y - v0.y);
		float m1 = (v2.x - v1.x) / (v2.y - v1.y);

		// calculate start and end scanlines
		const int yStart = (int)ceil(v0.y - 0.5f);
		const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

		float dzdy1 = ((v2.z - v0.z) / (v2.y - v0.y));
		float dzdy2 = ((v2.z - v1.z) / (v2.y - v1.y));

		for (int y = yStart; y < yEnd; y++)
		{
			// caluclate start and end points (x-coords)
			// add 0.5 to y value because we're calculating based on pixel CENTERS
			const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
			const float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

			// calculate start and end pixels
			const int xStart = (int)ceil(px0 - 0.5f);
			const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

			float pz1 = ((y - yStart) * dzdy1 + v0.z);
			float pz2 = ((y - yStart) * dzdy2 + v1.z);
			float zStep = (pz2 - pz1) / (px1 - px0);

			for (int x = xStart; x < xEnd; x++)	{
				float z = zStep * (x - xStart) + pz1;
				drawPointDepth(x,y,z, c.x, c.y, c.z);
			}
		}
	}

	void drawFlatBottomTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, Vec3 &c)
	{
		// calulcate slopes in screen space
		float m0 = (v1.x - v0.x) / (v1.y - v0.y);
		float m1 = (v2.x - v0.x) / (v2.y - v0.y);

		// calculate start and end scanlines
		const int yStart = (int)ceil(v0.y - 0.5f);
		const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

		float zxm1 = ((v1.z - v0.z) / (v1.y - v0.y));
		float zxm2 = ((v2.z - v0.z) / (v2.y - v0.y));

		for (int y = yStart; y < yEnd; y++)
		{
			// caluclate start and end points
			// add 0.5 to y value because we're calculating based on pixel CENTERS
			const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
			const float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

			// calculate start and end pixels
			const int xStart = (int)ceil(px0 - 0.5f);
			const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

			float pz1 = ((y - yStart) * zxm1 + v0.z);
			float pz2 = ((y - yStart) * zxm2 + v0.z);
			float zStep = (pz2 - pz1) / (px1 - px0);

			for (int x = xStart; x < xEnd; x++)	{
				float z = zStep * (x - xStart) + pz1;
				drawPointDepth(x, y, z, c.x, c.y, c.z);
			}
		}
	}



	//void DrawLineDepth(const Vec3& v0, const Vec3& v1, unsigned char r, unsigned char g, unsigned char b)
	//{
	//	Vec3

	//	float dx = v1.x - v0.x;
	//	float dy = v1.y - v0.y;

	//	if (dy == 0.0f && dx == 0.0f)
	//	{
	//	}
	//	else if (abs(dy) > abs(dx))
	//	{
	//		if (dy < 0.0f)
	//		{
	//			std::swap(v0, v1);
	//			dy = -dy;
	//		}

	//		const auto dv = (v1 - v0) / dy;
	//		for (auto v = v0; v.y < v1.y; v += dv)
	//		{
	//			const auto x = int(v.x);
	//			const auto y = int(v.y);
	//			if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
	//			{
	//				continue;
	//			}
	//			//if (testAndSetZ(x, y, v.z))
	//			//{
	//				drawPointDepth(v, r, g, b);
	//			//}
	//		}
	//	}
	//	else
	//	{
	//		if (dx < 0.0f)
	//		{
	//			std::swap(v0, v1);
	//			dx = -dx;
	//		}

	//		const auto dv = (v1 - v0) / dx;
	//		for (auto v = v0; v.x < v1.x; v += dv)
	//		{
	//			const auto x = int(v.x);
	//			const auto y = int(v.y);
	//			if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
	//			{
	//				continue;
	//			}
	//			//if (testAndSetZ(x, y, v.z))
	//			//{
	//				drawPointDepth(v, r, g, b);
	//			//}
	//		}
	//	}
	//}

	void resetZ() {
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
			zBuffer[i] = 0;// std::numeric_limits<float>::infinity();
		}
	}

	bool testAndSetZ(int x, int y, float depth) {
		if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
			float &val = zBuffer[x + y * SCREEN_WIDTH];
			if (depth > val) {
				val = depth;
				return true;
			}
		}
		return false;
	}

	float zBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
};