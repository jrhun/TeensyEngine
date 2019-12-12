#pragma once

#include "Pattern.h"
#include "../../data.h"

extern float GuiVars1;
extern float GuiVars2;
extern float GuiVars3;
extern float GuiVars4;
extern float GuiVars5;
extern float GuiVars6;

class PatternParametric : public _Pattern {
public:
	PatternParametric() : _Pattern("Parametric Lines") {}


	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
			gfx.fade(30);
			gfx.blur(90);
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
		offset += 0.1;

		float radius = myMap(Data::sampleAvg, 0, Data::audioThreshold * 2, 2.5, 1.8, true);

		centerA.x = radius * cos(angle + PI);
		centerA.z = radius * sin(angle + PI);

		centerB.x = radius * cos(angle + PI + PI);
		centerB.z = radius * sin(angle + PI + PI);

		angle += 0.005;
		if (angle >= TWO_PI)
			angle -= TWO_PI;
#if defined(ESP32) || defined(CORE_TEENSY)
		return (1000/60);
#else 
		return returnVal;
#endif
	}

	void drawLine(float t, uint8_t hue1, uint8_t hue2, uint8_t fade) {
		//Vec3 a(sin(t) * GuiVars1, cos(t / 2) * GuiVars2, cos(t) * GuiVars3); // 1.6/0.6/1.25
		//Vec3 b(sin(t) * GuiVars4, sin(t / 2) * GuiVars5, cos(t) * GuiVars6); // 0.87/1.25/1.15

		Vec3 a(sin(t) * 1.6, cos(t / 2) * 0.6, cos(t) * 1.25); // 1.6/0.6/1.25
		Vec3 b(sin(t) * 0.87, sin(t / 2) * 1.25, cos(t) * 1.15); // 0.87/1.25/1.15

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

class PatternParametric2 : public _Pattern {
public:
	PatternParametric2() : _Pattern("Parametric Spiral") {}


	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
			gfx.fade(30);
			gfx.blur(90);
		}

		gfx.resetZ();
		const int maxI = 100;
		float step = (PI) / (maxI - 1);
		m = Mat3::RotationY(angle)  * Mat3::RotationX(angle);
		for (int i = 0; i < maxI; i++) {
			float t = offset + i * step;
			drawLine(t, i / 1, i / 1 + 32, min(i * 8, 255));
		}


		offset += 0.05;

		float radius = myMap(Data::sampleAvg, 0, Data::audioThreshold * 2, 2.5, 1.8, true);

		centerA.x = radius * cos(angle + PI);
		centerA.z = radius * sin(angle + PI);

		centerB.x = radius * cos(angle + PI + PI);
		centerB.z = radius * sin(angle + PI + PI);

		angle += 0.005;
		if (angle >= TWO_PI)
			angle -= TWO_PI;

		return returnVal;
	}

	void drawLine(float t, uint8_t hue1, uint8_t hue2, uint8_t fade) {
		//https://math.stackexchange.com/questions/324527/do-these-equations-create-a-helix-wrapped-into-a-torus
		float fa = 2.5;
		float fb = 1.4;
		//float k = 0.05;
		float u = 2*t;
		float v = t*.75;
		float x1 = (fa + fb * cos(u)) * cos(v);
		float z1 = (fa + fb * cos(u)) * sin(v);
		float y1 = fb * sin(u);
		//v -= PI/16;
		u += PI ;
		float x2 = (fa + fb * cos(u)) * cos(v);
		float z2 = (fa + fb * cos(u)) * sin(v);
		float y2 = fb * sin(u);
		Vec3 a(x1, y1, z1);
		Vec3 b(x2, y2, z2);
		//Vec3 a(2.5 * cos(t+PI) + cos(t+PI/4)*cos(4*t+PI/2),		 sin(4*t+PI/2),		2.5 * sin(t) + sin(t + PI / 4)*sin(4*t+PI/2)); // 1.6/0.6/1.25
		//Vec3 b(2.5 * cos(t+PI) + cos(t)*cos(4*(t + PI/2)), sin(4*(t+PI/2)),	2.5 * sin(t) + sin(t)*sin(4*(t+PI/2))); // 0.87/1.25/1.15

		a = a * m;
		b = b * m;

		//a /= 4;
		//b /= 4;
		//a.z += 2;
		//b.z += 2;
		//Vec3 aa = a + centerA;
		//Vec3 ba = b + centerA;
		//engine.sst.TransformSphere(aa);
		//engine.sst.TransformSphere(ba);
		//gfx.drawLineDepth(aa, ba, c);

		//gfx.drawLineDepth(aa, ba, hue1, hue2, fade);
		//a += centerB;
		//b += centerB;
		engine.sst.TransformSphere(a);
		engine.sst.TransformSphere(b);
		gfx.drawLineDepth(a, b, hue1 + 60, hue2 + 60, fade);
	}

	Vec3 centerA = Vec3(0, 0, 2.5);
	Vec3 centerB = Vec3(0, 0, -2.5);
	Mat3 m;
	float offset = 0;
	float angle = 0;
};
