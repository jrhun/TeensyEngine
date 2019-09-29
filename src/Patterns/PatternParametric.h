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
		if (Pattern::useDefaultEffect) {
			gfx.fade(30);
			blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, 90);
		}
		gfx.resetZ();
		const int maxI = 100;
		float step = (PI) / (maxI - 1);
		m = Mat3::RotationY(angle)  * Mat3::RotationX(angle);
		for (int i = 0; i < maxI; i++) {
			float t = offset + i * step;
			//CRGB c1 = gfx.getColour(i / 2);
			//CRGB c2 = gfx.getColour(i / 2 + 40);
			//c1.nscale8_video(min(i*4, 255));
			//c2.nscale8_video(min(i * 4, 255));
			drawLine(t, i / 1, i / 1 + 32, min(i * 12, 255));
		}
		offset += 0.05;

		centerA.x = 2.5 * cos(angle + PI);
		centerA.z = 2.5 * sin(angle + PI);

		centerB.x = 2.5 * cos(angle + PI + PI);
		centerB.z = 2.5 * sin(angle + PI + PI);

		angle += 0.005;
		if (angle >= TWO_PI)
			angle -= TWO_PI;
		return returnVal;
	}

	void drawLine(float t, uint8_t hue1, uint8_t hue2, uint8_t fade) {
		Vec3 a(sin(t) * GuiVars1, cos(t / 2) * GuiVars2, cos(t) * GuiVars3); // 1.6/0.6/1.25
		Vec3 b(sin(t) * GuiVars4, sin(t / 2) * GuiVars5, cos(t) * GuiVars6); // 0.87/1.25/1.15

		a = a * m;
		b = b * m;

		//a /= 4;
		//b /= 4;
		//a.z += 2;
		//b.z += 2;
		Vec3 aa = a + centerA;
		Vec3 ba = b + centerA;
		engine.sst.TransformSphere(aa);
		engine.sst.TransformSphere(ba);
		//gfx.drawLineDepth(aa, ba, c);

		gfx.drawLineDepth(aa,ba, hue1, hue2, fade);
		a += centerB;
		b += centerB;
		engine.sst.TransformSphere(a);
		engine.sst.TransformSphere(b);
		gfx.drawLineDepth(a, b, hue1+60, hue2+60, fade);
	}

	Vec3 centerA = Vec3(0, 0, 0);
	Vec3 centerB = Vec3(0, 0, 0);
	Mat3 m;
	float offset = 0;
	float angle = 0;
};