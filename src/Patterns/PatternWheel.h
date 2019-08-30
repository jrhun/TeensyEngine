#pragma once

#include "Pattern.h"
#include "../../data.h"
#include "../Particle.h"

class PatternWheelBlur : public Pattern {
public:
	PatternWheelBlur() : Pattern("Wheel") {
	}


	uint8_t drawFrame() {

		for (uint8_t i = 0; i < 6; i++) {
			uint8_t x = offset + ((cos8(angle / 32) - 128) / 256.0) * myMap(i, 0, (6 - 1), -32, 32);
			uint8_t y = (SCREEN_HEIGHT / 2) + ((sin8(angle / 32) - 128) / 256.0) * myMap(i, 0, 6 - 1, -32, 32);
			CRGB c = gfx.getColour((i * 12) + random8(26));
			x %= SCREEN_WIDTH;
			gfx.putPixel(x, y, c);
			c = gfx.getColour((i * 12) + random8(26));
			gfx.putPixel(x + 1, y, c);
			c = gfx.getColour((i * 12) + random8(26));
			gfx.putPixel(x, y + 1, c);
			c = gfx.getColour((i * 12) + random8(26));
			gfx.putPixel(x + 1, y + 1, c);
		}

		if (Pattern::useDefaultEffect) {
			blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, 75); // 75 works well for blur
		}

		offset += 0.25;
		angle += 20;
		//offset %= SCREEN_WIDTH;
		return 0;
	}

	float offset = 0;
	int angle = 0;

};

class WheelPS : public ParticleSystem {
public:
	WheelPS() {}

	Particle getParticle() {
		static bool lastSide = true;
		Particle p;
		p.pos = Vec3(0.0f, 0.0f, 0.0f);
		float t;
		if (lastSide)	t = theta;
		else			t = TWO_PI - theta;
		//lastSide = !lastSide;
		//p.pos.x = 2 * cos(t);
		//p.pos.z = 2 * sin(t);
		//p.vel = { GuiVars1 - 1, GuiVars2 -1, GuiVars3 -1 };
		p.vel = { GuiVars1 - 1, GuiVars2 - 1, GuiVars3 - 1 };
		//p.vel *= 0.1; // -0.1 to 0.1
		p.vel *= 0.1;
		float rx = ((random8() - 127) / 1270.0); // gives value between -0.01 to 0.01
		float ry = ((random8() - 127) / 1270.0);
		p.vel.x += rx;
		p.vel.y += ry;
		// Vec3::getRandom() - Vec3(0.5f, 0.5f, 0.5f);
		//p.vel *= 0.1;
		p.alpha = 255.0f;
		p.col = gfx.getColour(random8(30) + (t)* 6);
		return p;
	}
	void incTheta(float d = 0.05) {
		theta += d;
		if (theta > TWO_PI)
			theta -= TWO_PI;
	}

	float theta = 0;

};

class PatternWheelPart : public Pattern {
public:
	PatternWheelPart() : Pattern("Wheel (Particles)") {
	}

	void start() {
		ps.init();
	}
	uint8_t drawFrame() {
		gfx.resetZ();
		if (Pattern::useDefaultEffect) {
			gfx.fade(128);
		}
		ps.addParticle();
		ps.addParticle();
		ps.run();
		ps.incTheta();
		return 0;
	}
	WheelPS ps;
};