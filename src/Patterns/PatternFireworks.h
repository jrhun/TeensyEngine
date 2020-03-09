#pragma once


#include "Pattern.h"


#include "PatternParticleSysOld.h"

//class fParticle : public Particle {
//public:
//	fParticle() {
//
//	}
//
//	fParticle(int _x, int _y, int h) {
//		x = _x;
//		y = _y;
//		vx = random(200) - 100;
//		vy = random(200) - 100;
//		ax = 0;
//		ay = 0;
//		alpha = 255;
//		mass = 0;
//		hue = (h + int(random(15))) % 255;
//	}
//
//	bool finished() {
//		return alpha < 10;
//	}
//	bool explode() {
//		if (alpha != 0 && vy > 10) {
//			alpha = 0;
//			return true;
//		}
//		return false;
//	}
//	void update() {
//		vx += ax;
//		vy += ay;
//		ax = 0;
//		ay = 0;
//		x = (x + vx + ROWS * STEP) % (ROWS * STEP);
//		y += vy;
//		if (y < (-STEP)) {
//			alpha = 0;
//		}
//		else if (mass == 0) {
//			if (alpha - 5 < 0) alpha = 0;
//			else               alpha -= 5;
//			vx = (vx * 235) >> 8;
//			vy = (vy * 235) >> 8;
//		}
//	}
//};
//
//class Firework {
//public:
//	std::vector<fParticle> part;
//	fParticle firework;
//
//	Firework() {
//		part.reserve(30);
//		reset();
//	}
//
//	void reset() {
//		firework.clear();
//		firework.x = random(4 * STEP, (ROWS - 4) * STEP);
//		firework.y = COLS * STEP;
//		firework.vx = random(80) - 40;
//		firework.vy = -random(150, 190);
//		firework.alpha = 255;
//		firework.mass = 1;
//		firework.hue = int(((random(32) + (4 * firework.x / STEP) + baseHue / baseHueScale)) % 256);
//	}
//
//	bool done() {
//		if (firework.alpha == 0 && part.empty()) {
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
//	void run() {
//		if (firework.alpha != 0) {
//			firework.ay += 1;
//			firework.update();
//			firework.show();
//			if (firework.explode()) {
//				for (int i = 0; i < 20; i++) {
//					part.push_back(fParticle(firework.x, firework.y, firework.hue));    // Add "num" amount of particles to the arraylist
//				}
//				firework.alpha = 0;
//			}
//		}
//
//		if (part.end() != part.begin()) {
//			for (std::vector<fParticle>::iterator it = part.end(); it != part.begin(); it--) {
//				fParticle *p = &(*it);
//				p->ay += 1;
//				p->update();
//				p->show();
//				if (p->finished()) {
//					it = part.erase(it);
//				}
//			}
//		}
//	}
//};
//
//class FireworkSystem {
//public:
//	enum { maxFireworks = 6 };
//	FireworkSystem() {
//		//      fireworks.reserve(maxFireworks);
//	}
//
//
//	//    std::vector<Firework> fireworks;
//	Firework fireworks[maxFireworks];
//
//
//	void run() {
//		for (uint8_t i = 0; i < maxFireworks; i++) {
//			Firework *f = &fireworks[i];
//			if (f->done()) {
//				if (random8(10) < 1) {
//					f->reset();
//				}
//			}
//			f->run();
//
//		}
//	}
//};


class PatternFireworks : public _Pattern {
public:
	PatternFireworks()
		: _Pattern("Fireworks") {}

	uint8_t drawFrame() {
		_Pattern::drawFrame();

		if (!_Pattern::useCustomEffect) {
			gfx.fade(100);
		}

		//separationPerceptionRadius = myMap(*beat, 0, 255, 0, 350);// GuiVars1 * 200;

		fs.run();


		return returnVal;
	}

	FireworkSystem fs;

};