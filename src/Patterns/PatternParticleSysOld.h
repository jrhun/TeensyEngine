#pragma once

#include "Pattern.h"

#include <list>
#include <vector>

int16_t gravity = 1;
const uint16_t STEP = 256;
const uint16_t MAX_PARTICLES = 150;
const uint16_t alignmentPerceptionRadius = 1000;
const uint16_t cohesionPerceptionRadius = 1000;
const uint16_t separationPerceptionRadius = 400;
const uint16_t maxForce = 10;
const uint16_t maxSpeed = 150;

uint16_t baseHue = 0;
uint8_t baseHueScale = 4;

uint16_t sqrt32(uint32_t x) {
	if (x == 0 || x == 1) return x;

	uint32_t left = 1;
	uint32_t right = x / 2 + 1;
	uint32_t res;

	while (left <= right) {
		uint32_t mid = left + ((right - left) / 2);
		if (mid <= x / mid) {
			left = mid + 1;
			res = mid;
		}
		else {
			right = mid - 1;
		}
	}

	return uint16_t(res);
}

uint16_t distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	return sqrt32(uint32_t((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
	//  return sqrt32(uint32_t( (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) ));
}

uint32_t distanceSq(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	return (uint32_t((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

uint16_t calcForce(uint16_t x, uint16_t y) {
	return sqrt32(uint32_t(x * x + y * y));
}
uint16_t calcForce(int16_t x, int16_t y) {
	return sqrt32(uint32_t(x * x + y * y));
}
uint32_t calcForceSq(uint16_t x, uint16_t y) {
	return uint32_t(x * x + y * y);
}


class ParticleOld {
public:
	int16_t x;
	int16_t y;
	int16_t vx;
	int16_t vy;
	int16_t ax;
	int16_t ay;
	uint8_t hue;
	uint8_t alpha;
	uint8_t mass;
	uint16_t bitMask;

	ParticleOld() : mass(1), x(0), y(0), vx(0), vy(0), ax(0), ay(0), alpha(0), hue(0), bitMask(0) {    }

	ParticleOld(ParticleOld *p) {
		x = p->x; y = p->y;
		vx = p->vx; vy = p->vy;
		ax = p->ax; ay = p->ay;
		hue = p->hue;
		alpha = p->alpha;
		mass = p->mass;
	}

	void clear() {//(uint16_t x, uint16_t y, int16_t vx, int16_t vy, uint8_t hue, uint8_t alpha) {
		x = 0; y = 0; vx = 0; vy = 0; ax = 0; ay = 0; hue = 0; alpha = 0; mass = 1;
	}

	bool isDead() {
		return (alpha < 10);
	}

	void attract(uint16_t attractX, uint16_t attractY) {
		int16_t sx = attractX - x;
		if (sx > ((SCREEN_WIDTH / 2) * STEP))
			sx -= SCREEN_WIDTH * STEP;
		else if (sx < -((SCREEN_WIDTH / 2) * STEP))
			sx += SCREEN_WIDTH * STEP;
		int16_t sy = attractY - y;
		uint16_t m = calcForce(sx, sy);
		sx = sx * maxForce / (m + 1);
		sy = sy * maxForce / (m + 1);
		applyForce(sx, sy);
	}


	void applyForceMass(int16_t fx, int16_t fy) {
		// scale based on mass. a mass of 1 means 255/255, 255 means ~1/255
		//  mass
		fx = scale(fx, 255 - mass); fy = scale(fy, 255 - mass);
		ax += fx; ay += fy;
	}

	void applyForce(int16_t fx, int16_t fy) {
		ax += fx;
		ay += fy;
		uint16_t force = calcForce(ax, ay);
		if (force > maxForce) {
			ax = (ax * maxForce) / force;
			ay = (ay * maxForce) / force;
		}
	}

	void allFlock(ParticleOld *p) {
		int32_t alignX = 0, alignY = 0;
		int32_t cohX = 0, cohY = 0;
		int32_t sepX = 0, sepY = 0;
		uint8_t totalA = 0;
		uint8_t totalC = 0;
		uint8_t totalS = 0;
		for (uint16_t i = 0; i < MAX_PARTICLES; i++) {
			ParticleOld *other = &p[i];
			if (other->isDead()) continue;
			if (other == this) continue;
			if (!(this->bitMask & other->bitMask)) continue;
			uint16_t d = distance(this->x, this->y, other->x, other->y);
			if ((d < alignmentPerceptionRadius)) {
				alignX += other->vx;
				alignY += other->vy;
				totalA++;
			}
			if (d < cohesionPerceptionRadius) {
				cohX += other->x;
				cohY += other->y;
				totalC++;
			}
			if (d < separationPerceptionRadius) {
				int16_t dx = this->x - other->x;
				int16_t dy = this->y - other->y;
				dx = dx * (separationPerceptionRadius - d) / separationPerceptionRadius;
				//          dx = dx * (separationPerceptionRadius - d) / separationPerceptionRadius;
				dy = dy * (separationPerceptionRadius - d) / separationPerceptionRadius;
				//          dy = dy * (separationPerceptionRadius - d) / separationPerceptionRadius;
				sepX += dx;
				sepY += dy;
				totalS++;
			}
		}
		if (totalA > 0) {
			alignX /= totalA;
			alignY /= totalA;
			uint16_t m = calcForce(int16_t(alignX), int16_t(alignY));
			alignX = alignX * maxSpeed / (m + 1); //normalise to maxSpeed
			alignY = alignY * maxSpeed / (m + 1);
			alignX -= this->vx;
			alignY -= this->vy;
			//        while (calcForceSq(uint16_t(alignX), uint16_t(alignY)) > maxForce * maxForce) {
			//          alignX = alignX * 95 / 100;
			//          alignY = alignY * 95 / 100;
			//        }
		}
		if (totalC > 0) {
			cohX /= totalC;// average vector away from other particles
			cohY /= totalC;
			cohX -= this->x;
			cohY -= this->y;
			uint16_t m = calcForce(int16_t(cohX), int16_t(cohY));
			cohX = cohX * maxSpeed / (m + 1); //normalise to maxSpeed
			cohY = cohY * maxSpeed / (m + 1);
			cohX -= this->vx;
			cohY -= this->vy;
			//        while (calcForceSq(uint16_t(cohX), uint16_t(cohY)) > maxForce * maxForce) {
			//          cohX = cohX * 95 / 100;
			//          cohY = cohY * 95 / 100;
			//        }
		}
		if (totalS > 0) {
			sepX /= totalS;
			sepY /= totalS;
			uint16_t sepSpeed = calcForce(int16_t(sepX), int16_t(sepY));
			if (sepSpeed < maxSpeed) {
				sepX = (sepX * maxSpeed) / (sepSpeed + 1); //normalise to maxSpeed
				sepY = (sepY * maxSpeed) / (sepSpeed + 1);
			}
			sepX -= this->vx;  // get vector from my vel to other
			sepY -= this->vy;
			//        while (calcForceSq(int16_t(sepX), int16_t(sepY)) > maxForce * maxForce) {
			//          sepX = sepX * 95 / 100;
			//          sepY = sepY * 95 / 100;
			//        }
		}
		//      applyForce((alignX + cohX + sepX) / 3, (alignY + cohY + sepY) / 3);
		//      applyForce(alignX , alignY);
		//      applyForce(cohX, cohY);
		applyForce(sepX, sepY);

	}

	void applyGravity() {
		int16_t gy = (gravity * 255) / mass;
		applyForce(0, gy);
	}

	void update(bool wrap = true) {

		vx += ax;
		vy += ay; // add acceleration
		ax = 0; ay = 0; // reset accel

		uint16_t _speed = calcForce(vx, vy);
		if (_speed > maxSpeed) {
			vx = vx * maxSpeed / _speed;
			vy = vy * maxSpeed / _speed;
		}
		x = (x + vx + SCREEN_WIDTH * STEP) % (SCREEN_WIDTH * STEP);
		if (wrap)
			y = (y + vy + SCREEN_HEIGHT * STEP) % (SCREEN_HEIGHT * STEP);
		else {
			y += vy;
			if ((y <= 64) or y > STEP * SCREEN_HEIGHT) {
				alpha = 1;
			}
		}

		alpha -= 1;
		setBitmask();
	}

	void setBitmask() {
		// use simple / 16 for bitmask
		// offset by half of bitmask width (8 = 2024) to expand bitmask a little
		uint8_t bx1 = (x + 2 * STEP) / STEP / 8; // 0 to 8
		uint8_t by1 = (y + 2 * STEP) / STEP / 8; // 0 to 4
		uint8_t bx2 = (x - 2 * STEP) / STEP / 8; // 0 to 8
		uint8_t by2 = (y - 2 * STEP) / STEP / 8; // 0 to 4
		bitMask = 0;
		if (bx1 < 8)
			bitMask |= (0b1 << bx1);
		if (by1 < 4)
			bitMask |= (0b1 << by1) << 8;
		if (bx2 > 0)
			bitMask |= (0b1 << bx2);
		if (by2 > 0)
			bitMask |= (0b1 << by2) << 8;
	}

	int16_t scale(int16_t x, uint8_t s) {
		return (x * (1 + s) / 256);
	}

	void draw(uint8_t x, uint8_t y, CRGB color, uint8_t amount) {
		color.nscale8_video(amount*4);
		gfx.blendPixel(x, y, color, amount);
	}

	void show(bool mapX = false) {
		uint16_t mult = STEP;
		uint8_t ix = (x) / (mult);
		uint8_t iy = (y) / (mult);
		uint16_t dx = mult - (x % mult);
		uint16_t dy = mult - (y % mult);

		CRGB c;
		if (mapX) {
			uint8_t h = (hue + (uint8_t)myMap(x, 0, SCREEN_WIDTH * mult, 0, 255)) % 256;
			c = gfx.getColour(Data::getHue()+h);
			//c = CHSV(h, myMap(iy, 0, SCREEN_HEIGHT, 128, 255), 255);
		}
		else {
			c = gfx.getColour(Data::getHue()+hue);
			//c = CHSV(hue + baseHue, 255, 255);//map(iy, 0 , SCREEN_WIDTH*STEP , 255, 128));
		}

		uint32_t temp = (uint32_t)dx * dy * alpha >> 16; // STEP * STEP
		draw((ix), (iy), c, temp);
		if (ix < SCREEN_WIDTH) {
			temp = (uint32_t)(mult - dx) * dy * alpha >> 16;
			draw((ix + 1), (iy), c, temp);
		}
		if (ix < SCREEN_WIDTH and iy < SCREEN_HEIGHT) {
			temp = (uint32_t)(mult - dx) * (mult - dy) * alpha >> 16;
			draw((ix + 1), (iy + 1), c, temp);
		}
		if (iy < SCREEN_HEIGHT) {
			temp = (uint32_t)dx * (mult - dy) * alpha >> 16;
			draw((ix), (iy + 1), c, temp);
		}
	}
};

ParticleOld _particles[MAX_PARTICLES];

class ParticleSystemOld {
public:
	uint16_t x;
	uint16_t y;
	uint8_t pos;
	uint8_t counter;

	ParticleSystemOld() : x(SCREEN_WIDTH * STEP / 4), y(SCREEN_HEIGHT * STEP / 2), pos(0), counter(0) {
		//      _particles.reserve(MAX_PARTICLES);

	}
	void addParticle() {
		ParticleOld *p = getNextParticle();
		if (!p) return;
		//      Particle p = new Particle();
		p->clear();
		p->x = pos * 32;
		pos += 2;
		pos %= 512;
		//      p->y = y;
		p->y = SCREEN_HEIGHT * STEP / 2;// map(sin8(p->x / 64), 0, 255, (32 * 256 / 2) - 256 * 6, (32 * 256 / 2) + 256 * 6);
		p->vx = random8(30) - 15;
		p->vy = random8(30) - 15;
		p->alpha = 255;//random(25, 255);
		p->hue = (random8(64) + baseHue / baseHueScale) % 256;

		//      _particles.push_back(p);
	}
	ParticleOld *getNextParticle() {
		for (uint16_t i = 0; i < MAX_PARTICLES; i++) {
			if (_particles[i].isDead())
				return &_particles[i];
		}
		return NULL;
	}
	void applyForces() {

	}
	void applyGravity() {

	}

	void run() {
		for (uint16_t i = 0; i < MAX_PARTICLES; i++) {
			ParticleOld *p = &_particles[i];
			if (p->isDead()) {
				//          _particles.remove(i);
				continue;
			}
			else {
				//          p->follow();
				//          p->flock(_particles);
				p->update();
				p->alpha--;
				p->show();
				//          Serial.printf("particle %i at x:%i, y:%i, alpha:%i", i, p->x, p->y, p->alpha);
			}
		}
	}

};

class FlockSystem : public ParticleSystemOld {
public:
	FlockSystem() {}

	void addParticle() {
		ParticleOld *p = getNextParticle();
		if (!p) return;
		p->clear();
		p->x = random16(SCREEN_WIDTH * STEP);
		p->y = random16(SCREEN_HEIGHT * STEP);
		p->vx = random8(50) - 25;
		p->vy = random8(50) - 25;
		p->alpha = random8(128, 255);
		p->hue = (random8(64) + baseHue / baseHueScale) % 256;
	}

	void applyForces() { //flock
		for (uint16_t i = 0; i < MAX_PARTICLES; i++) {
			ParticleOld *p = &_particles[i];
			if (p->isDead()) {
				continue;
			}
			else {
				p->allFlock(_particles);
			}
		}
	}
};

class fParticle : public ParticleOld {
public:
	fParticle() {

	}

	fParticle(int _x, int _y, int h) {
		x = _x;
		y = _y;
		vx = random8(200) - 100;
		vy = random8(200) - 100;
		ax = 0;
		ay = 0;
		alpha = 255;
		mass = 0;
		hue = (h + int(random8(15))) % 255;
	}

	bool finished() {
		return alpha < 10;
	}
	bool explode() {
		if (alpha != 0 && vy > 10) {
			alpha = 0;
			return true;
		}
		return false;
	}
	void update() {
		vx += ax;
		vy += ay;
		ax = 0;
		ay = 0;
		x = (x + vx + SCREEN_HEIGHT * STEP) % (SCREEN_HEIGHT * STEP);
		y += vy;
		if (y < (-STEP)) {
			alpha = 0;
		}
		else if (mass == 0) {
			if (alpha - 5 < 0) alpha = 0;
			else               alpha -= 5;
			vx = (vx * 235) >> 8;
			vy = (vy * 235) >> 8;
		}
	}
};

class Firework {
public:
	std::vector<fParticle> part;
	fParticle firework;

	Firework() {
		part.reserve(30);
		reset();
	}

	void reset() {
		firework.clear();
		firework.x = random16(4 * STEP, (SCREEN_HEIGHT - 4) * STEP);
		firework.y = SCREEN_WIDTH * STEP;
		firework.vx = random8(80) - 40;
		firework.vy = -random8(150, 190);
		firework.alpha = 255;
		firework.mass = 1;
		firework.hue = int(((random8(32) + (4 * firework.x / STEP) + baseHue / baseHueScale)) % 256);
	}

	bool done() {
		if (firework.alpha == 0 && part.empty()) {
			return true;
		}
		else {
			return false;
		}
	}
	void run() {
		if (firework.alpha != 0) {
			firework.ay += 1;
			firework.update();
			firework.show();
			if (firework.explode()) {
				for (int i = 0; i < 20; i++) {
					part.push_back(fParticle(firework.x, firework.y, firework.hue));    // Add "num" amount of _particles to the arraylist
				}
				firework.alpha = 0;
			}
		}

		if (part.end() != part.begin()) {
			for (std::vector<fParticle>::iterator it = part.end(); it != part.begin(); it--) {
				fParticle *p = &(*it);
				p->ay += 1;
				p->update();
				p->show();
				if (p->finished()) {
					it = part.erase(it);
				}
			}
		}
	}
};

class FireworkSystem : public ParticleSystemOld {
public:
	enum { maxFireworks = 6 };
	FireworkSystem() {
		//      fireworks.reserve(maxFireworks);
	}


	//    std::vector<Firework> fireworks;
	Firework fireworks[maxFireworks];

	void addParticle() {
		//      if (random8(10) < 2) { // and fireworks.size() < maxFireworks) {
		//        //        fireworks.push_back(Firework());
		//        for (uint8_t i = 0; i < maxFireworks; i++) {
		//          if (fireworks[i].done()) {
		//            fireworks[i].reset();
		//          }
		//        }
		//      }
	}

	void run() {
		//      if (fireworks.end() == fireworks.begin()) return;
		//      for (std::vector<Firework>::iterator it = fireworks.end(); it != fireworks.begin(); it--) {
		//        Firework *f = &(*it);
		//        f->run();
		//        if (f->done())
		//          it = fireworks.erase(it);
		//      }
		for (uint8_t i = 0; i < maxFireworks; i++) {
			Firework *f = &fireworks[i];
			if (f->done()) {
				if (random8(10) < 1) {
					f->reset();
				}
			}
			f->run();

		}
	}
};

class WheelSystem : public ParticleSystemOld {
public:
	uint16_t x = 0;
	uint16_t offset = 0;

	void applyForces() {

	}
	void addParticle() {
		const uint8_t maxSpokes = 6;
		for (uint8_t i = 0; i < maxSpokes; i++) {
			ParticleOld *p = getNextParticle();
			if (!p) return;
			p->clear();
			p->x = offset + x + (cos8(x / 32) - 128) * (int8_t)myMap(i, 0, (maxSpokes - 1), -32, 32);
			p->y = (SCREEN_HEIGHT / 2) * STEP + (sin8(x / 32) - 128) * (int8_t)myMap(i, 0, maxSpokes - 1, -32, 32);
			p->vy = random8(20) - 10;
			p->vx = random8(20) - 10;
			p->hue = (baseHue / baseHueScale + random8(26) + i * 12) % 256;
			p->alpha = 255;//random8(25, 255);
		}
	}
	void run() {
		offset += 5;
		offset %= SCREEN_WIDTH * STEP;
		x += 30;
		x %= SCREEN_WIDTH * STEP;
		for (uint16_t i = 0; i < MAX_PARTICLES; i++) {
			ParticleOld *p = &_particles[i];
			if (p->isDead()) {
				continue;
			}
			else {
				//p->allFlock(_particles);
				p->update();
				if (p->alpha - 10 < 0) p->alpha = 0;
				else {
					p->alpha -= 10;
					p->show();
					ParticleOld offsetP = ParticleOld(p);
					offsetP.x = (offsetP.x + SCREEN_WIDTH * STEP / 2) % (SCREEN_WIDTH * STEP);
					offsetP.y = SCREEN_HEIGHT * STEP - 1 - offsetP.y;
					offsetP.show();
				}

			}
		}
	}
};

class AttractorSystem : public ParticleSystemOld {
public:
	uint16_t x = 0;
	uint16_t offset = 0;
	void addParticle() {
		const uint8_t maxSpokes = 6;
		for (uint8_t i = 0; i < maxSpokes; i++) {
			ParticleOld *p = getNextParticle();
			if (!p) return;
			p->clear();
			p->x = random16(SCREEN_WIDTH * STEP);
			p->y = random16(SCREEN_HEIGHT * STEP);
			p->vy = 0;
			p->vx = 0;
			p->hue = (baseHue / baseHueScale + random8(26) + i * 12) % 256;
			p->alpha = 255;//random(25, 255);
		}
	}
	void run() {
		offset += 5;
		offset %= SCREEN_WIDTH * STEP;
		//      x += 30;
		//      x %= SCREEN_WIDTH * STEP;
		for (uint16_t i = 0; i < MAX_PARTICLES; i++) {
			ParticleOld *p = &_particles[i];
			if (p->isDead()) {
				continue;
			}
			else {
				p->attract(offset, (SCREEN_HEIGHT / 2) * STEP);
				p->update();
				if (p->alpha - 9 < 0) p->alpha = 0;
				else {
					p->alpha -= 9;
					uint8_t a = p->alpha;
					p->alpha = 255 - p->alpha;
					p->show();
					p->alpha = a;
					//            if (distance(offset, (SCREEN_HEIGHT/2)*STEP, p->x, p->y) < 400) {
					//              p->alpha = 0;
					//            }
				}
			}
		}
	}
};



//ParticleSystemOld ps;
//RisingSystem ps;
//FlockSystem ps;
//FireworkSystem ps;
//FireSystem ps;
WheelSystem ps;
//AttractorSystem ps;


class PatternWheelPart : public _Pattern {
public:
	PatternWheelPart() : _Pattern("Wheel Particles") {}

	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
			gfx.fade(255);
		}
		ps.applyForces();
		ps.addParticle();
		ps.run();
		baseHue = Data::getHue();
		return returnVal;
	}
};