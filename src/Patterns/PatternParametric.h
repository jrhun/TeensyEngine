#pragma once

#include "Pattern.h"

class PatternParametric : public Pattern {
public:
	PatternParametric() : Pattern("Parametric Lines") {}


	uint8_t drawFrame() {
		gfx.fade(128);
		gfx.resetZ();
		const int maxI = 128;
		float step = (PI) / (maxI - 1);
		for (int i = 0; i < maxI; i++) {
			float t = offset + i * step;
			CRGB c = gfx.getColour(i / 2);
			drawLine(t, c);
		}
		offset += 0.1;
		center.x = 2 * cos(offset);
		center.z = 2 * sin(offset);
		return 0;
	}

	void drawLine(float t, CRGB &c) {
		Vec3 a(sin(t) * 0.5, cos(t / 2) * 1, cos(t) * 1.5);
		Vec3 b(sin(t) * 1.5, sin(t / 2) * 0.5, cos(t) * 1.5);
		//a /= 4;
		//b /= 4;
		a.z += 2;
		b.z += 2;
		//a += center;
		//b += center;
		engine.sst.TransformSphere(a);
		engine.sst.TransformSphere(b);
		gfx.drawLineDepth(a, b, c);
	}

	Vec3 center;
	float offset;
};