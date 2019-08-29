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
			uint8_t x = offset + ((cos8(angle / 32) - 128)/256.0) * myMap(i, 0, (6 - 1), -32, 32);
			uint8_t y = (SCREEN_HEIGHT / 2) + ((sin8(angle / 32) - 128)/256.0) * myMap(i, 0, 6 - 1, -32, 32);
			CRGB c = gfx.getColour((i * 12) + random8(26));
			x %= SCREEN_WIDTH;
			gfx.putPixel(x, y, c);
			c = gfx.getColour((i * 12) + random8(26));
			gfx.putPixel(x+1, y, c);
			c = gfx.getColour((i * 12) + random8(26));
			gfx.putPixel(x, y+1, c);
			c = gfx.getColour((i * 12) + random8(26));
			gfx.putPixel(x+1, y+1, c);
		}

		if (Pattern::useDefaultEffect) {
			blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, 75); // 75 works well for blur
		}

		offset += 0.4;
		angle += 30;
		//offset %= SCREEN_WIDTH;
		return 0;
	}

	float offset = 0;
	int angle = 0;

};

//class WheelPS : public ParticleSystem {
//public:
//	WheelPS() {}
//
//	Particle getParticle() {
//		Particle p;
//		p.pos = Vec3(0.0f, 0.0f, 0.0f );
//
//		return p;
//	}
//
//};

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
		return 0;
	}
	ParticleSystem ps;
};