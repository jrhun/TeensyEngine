#pragma once

#include "Pattern.h"
#include "../../data.h"

extern float GuiVars1;
extern float GuiVars2;
extern float GuiVars3;
extern float GuiVars4;
extern float GuiVars5;
extern float GuiVars6;

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
		center.x = 2 * cos(offset+ PI);
		center.z = 2 * sin(offset);
		return 0;
	}

	void drawLine(float t, CRGB &c) {
		Vec3 a(sin(t) * GuiVars1, cos(t / 2) * GuiVars2, cos(t) * GuiVars3);
		Vec3 b(sin(t) * GuiVars4, sin(t / 2) * GuiVars5, cos(t) * GuiVars6);
		//a /= 4;
		//b /= 4;
		//a.z += 2;
		//b.z += 2;
		float xOff = 2 * cos(angle + PI);
		float zOff = 2 * sin(angle + PI);
		v.x += xOff;
		v.z += zOff;
		engine.sst.TransformSphere(a);
		engine.sst.TransformSphere(b);
		gfx.drawLineDepth(a, b, c);
	}

	Vec3 center;
	float offset;
};