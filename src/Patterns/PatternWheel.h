#pragma once

#include "Pattern.h"
#include "../../data.h"
#include "../Particle.h"

class PatternWheelBlur : public _Pattern {
public:
	PatternWheelBlur() : _Pattern("Wheel") {
	}


	uint8_t drawFrame() {

		for (uint8_t i = 0; i < 6; i++) {
			uint8_t x = offset + ((cos8(angle / 32) - 128) / 256.0) * myMap(i, 0, (6 - 1), -30, 30);
			uint8_t y = (SCREEN_HEIGHT / 2) + ((sin8(angle / 32) - 128) / 256.0) * myMap(i, 0, 6 - 1, -30, 30);
			CRGB c = gfx.getColour((i * 12)  + random8(10));
			x %= SCREEN_WIDTH;
			gfx.putPixel(x, y, c);
			c = gfx.getColour((i * 12) + random8(10));
			gfx.putPixel(x + 1, y, c);
			c = gfx.getColour((i * 12) + random8(10));
			gfx.putPixel(x, y + 1, c);
			c = gfx.getColour((i * 12) + random8(10));
			gfx.putPixel(x + 1, y + 1, c);
		}

		if (_Pattern::useDefaultEffect) {
			gfx.blur(75); // 75 works well for blur
		}

		offset += 0.25;
		angle += 20;
		//offset %= SCREEN_WIDTH;
		
		return returnVal;
	}

	float offset = 0;
	int angle = 0;

};


//class PatternWheelBlur2 : public _Pattern {
//public:
//	PatternWheelBlur2() : _Pattern("Wheel 2(PS)") {
//	}
//
//	class WheelSystem : public ParticleSystem {
//	public:
//		uint16_t x = 0;
//		uint16_t offset = 0;
//
//		Particle getParticle() {
//			const uint8_t maxSpokes = 6;
//			static uint8_t nextSpoke = 0;
//			Particle p;
//			p.pos.x = offset + x + (cos8(x / 32) - 128) * map(i, 0, (maxSpokes - 1), -32, 32);
//			p.pos.y = (SCREEN_HEIGHT / 2) * STEP + (sin8(x / 32) - 128) * map(i, 0, maxSpokes - 1, -32, 32);
//			p.vel.y = random(20) - 10;
//				p->vx = random(20) - 10;
//				p->hue = (baseHue / baseHueScale + random8(26) + i * 12) % 256;
//				p->alpha = 255;//random(25, 255);
//			}
//			nextSpoke++;
//			nextSpoke %= maxSpokes;
//		}
//		void run() {
//			offset += 5;
//			offset %= SCREEN_WIDTH * STEP;
//			x += 30;
//			x %= SCREEN_WIDTH * STEP;
//			for (uint16_t i = 0; i < MAX_PARTICLES; i++) {
//				Particle *p = &particles[i];
//				if (p->isDead()) {
//					continue;
//				}
//				else {
//					p->allFlock(particles);
//					p->update();
//					if (p->alpha - 10 < 0) p->alpha = 0;
//					else {
//						p->alpha -= 10;
//						p->show();
//						Particle offsetP = Particle(p);
//						offsetP.x = (offsetP.x + SCREEN_WIDTH * STEP / 2) % (SCREEN_WIDTH * STEP);
//						offsetP.y = SCREEN_HEIGHT * STEP - 1 - offsetP.y;
//						offsetP.show();
//					}
//
//				}
//			}
//		}
//	} ps;
//
//	uint8_t drawFrame() {
//
//		for (uint8_t i = 0; i < 6; i++) {
//			uint8_t x = offset + ((cos8(angle / 32) - 128) / 256.0) * myMap(i, 0, (6 - 1), -30, 30);
//			uint8_t y = (SCREEN_HEIGHT / 2) + ((sin8(angle / 32) - 128) / 256.0) * myMap(i, 0, 6 - 1, -30, 30);
//			CRGB c = gfx.getColour((i * 12) + random8(10));
//			x %= SCREEN_WIDTH;
//			gfx.putPixel(x, y, c);
//			c = gfx.getColour((i * 12) + random8(10));
//			gfx.putPixel(x + 1, y, c);
//			c = gfx.getColour((i * 12) + random8(10));
//			gfx.putPixel(x, y + 1, c);
//			c = gfx.getColour((i * 12) + random8(10));
//			gfx.putPixel(x + 1, y + 1, c);
//		}
//
//		if (_Pattern::useDefaultEffect) {
//			gfx.blur(75); // 75 works well for blur
//		}
//
//		offset += 0.25;
//		angle += 20;
//		//offset %= SCREEN_WIDTH;
//
//		return returnVal;
//	}
//
//	float offset = 0;
//	int angle = 0;
//
//};

class particleFountain : public ParticleSystem {
public:
	particleFountain() {}

	Particle getParticle() {
		static bool lastSide = true;
		Particle p;
		p.pos = Vec3(0.0f, 1.5f, 0.0f);
		float t;
		if (lastSide)	t = theta;
		else			t = TWO_PI - theta;
		//lastSide = !lastSide;
		p.pos.x = 2.1 * cos(t);
		p.pos.z = 2.1 * sin(t);

		p.vel.x = 0.5 * cos(t*6);
		p.vel.z = 0.5 * sin(t*6);
		p.vel.y = -1.45;// (GuiVars2 - 1);
		//p.vel = { GuiVars1 - 1, GuiVars2 - 1, GuiVars3 - 1 };
		//p.vel *= 0.1; // -0.1 to 0.1
		p.vel *= 0.1;
		Vec3 ran = Vec3::getRandom(); //-1 to 1
		ran *= 0.02;
		p.vel += ran;
		//p.vel *= 0.1;
		p.acc = Vec3(0.0f, 0.0f, 0.0f);
		p.alpha = 255.0f;
		p.col = gfx.getColour(random8(30) + (t)* 6);
		p.hue = int(Data::getHue() + random8(30) + (t) * 6) % 256;
		p.mass = 1.0f;
		return p;
	}
	void incTheta(float d = 0.02) {
		theta += d;
		if (theta > TWO_PI)
			theta -= TWO_PI;
	}

	float theta = 0;

};

class PatternFountain : public _Pattern {
public:
	PatternFountain() : _Pattern("Particle fountain") {
	}

	void start() {
		ps.init();
	}
	uint8_t drawFrame() {
		gfx.resetZ();
		if (_Pattern::useDefaultEffect) {
			gfx.fade(128);
		}
		//ps.applyForce(Vec3(0.0f, 0.005*(GuiVars6-1), 0.0f)); // gravity
		ps.applyForce(Vec3(0.0f, 0.0075, 0.0f)); // gravity
		ps.addParticle();
		//ps.addParticle();
		ps.run();
		ps.incTheta();
		return returnVal;
	}
	particleFountain ps;
};
