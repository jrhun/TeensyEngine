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

		if (!_Pattern::useCustomEffect) {
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

		float vy = myMap(Data::sampleAvg, 0, Data::audioThreshold * 2,-1.4, -1.8, true);

		p.vel.x = 0.5 * cos(t*6); //initial 0.5
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

	virtual void decreaseLife(Particle &p) {
		p.alpha -= 1.5f;
	}

	float theta = 0;

};

class PatternFountain : public _Pattern {
public:
	PatternFountain() : _Pattern("Particle fountain") {
	}

	void start() {
		ps.init();
		ps.limit = true;
	}
	void stop() {
		particles.clear();
	}
	uint8_t drawFrame() {
		gfx.resetZ();
		if (!_Pattern::useCustomEffect) {
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

class ParticleSystemWater : public _Pattern {
public: 
	ParticleSystemWater() : _Pattern("Water") {
		//addParticles(64);
	}

	void stop() {
		particles.clear();
	}

	uint8_t drawFrame() {
		gfx.resetZ();
		if (!_Pattern::useCustomEffect) {
			gfx.fade(80);
		}
		addParticles(8);
		update();
		run();

		return returnVal;
	}

	void addParticles(uint8_t n) {
		float step = TWO_PI / numParticles;
		static float theta = 0;
		static uint8_t j = 0;

		for (uint8_t i = 0; i < n; i++) {
			Particle p;
			p.pos.x = 2.1 * cos(theta) * cos(Data::roll * PI / 180.0f);
			p.pos.z = 2.1 * sin(theta) * cos(Data::pitch * PI / 180.0f);
			p.pos.y = sin(Data::pitch* PI / 180.0f) * p.pos.z + sin(Data::roll* PI / 180.0f) * p.pos.x;// +0.2f * i / 2;

			p.vel = Vec3::getRandom();
			p.vel *= 0.02;

			p.acc = Vec3(0.0f, 0.0f, 0.0f);
			p.alpha = 255.0f;

			p.mass = theta;

			p.hue = j + random8(20);
			
			//if (i > numParticles / 2)
			//	p.hue = i*2;
			//else
			//	p.hue = numParticles - i * 2;
			particles.push_back(p);
			
		}
		theta += step;
		j += 1;
		if (theta >= TWO_PI)
			theta -= TWO_PI;
	}

	void update() {
		float step = TWO_PI / numParticles;
		static float theta = 0;
		float lastTheta = 0;
		uint8_t count = 0;

		for (Particle &p : particles) {
			Vec3 desired; 
			float theta = p.mass;
			desired.x = 2.1 * cos(theta) * cos(Data::roll * PI / 180.0f);
			desired.z = 2.1 * sin(theta) * cos(Data::pitch * PI / 180.0f);
			desired.y = sin(Data::pitch* PI / 180.0f) * desired.z + sin(Data::roll* PI / 180.0f) * desired.x;
			if (theta == lastTheta) {
				count++;
				//desired.y += 0.4f * count/2;
			}
			else {
				count = 0;
			}
			
			Vec3 v = Vec3::getRandom();
			v *= 0.005;

			Vec3 dir = desired - p.pos;
			dir = dir * 0.008f + v;
			p.applyForce(dir);

			if (p.alpha - 2.5f < 0) p.alpha = 0;
			else p.alpha -= 2.5f;
			lastTheta = theta;
		}
	}

	
	void run() {
		float step = TWO_PI / numParticles;
		float theta = 0;

		auto end = std::remove_if(particles.begin(), particles.end(), [this](Particle &p) {
			// update particle
			p.update();

			// render particle
			Vec3 pos = p.pos;// *m;
			//pos.z += 2;

			engine.sst.TransformSphere(pos);
			//gfx.putPixel(pos.x, pos.y, p.col.nscale8_video(p.alpha));
			float dis = myMap(p.pos.Len(), 2, 4, 255, 128, true);

			CRGB c;
			if (Data::paletteIndex == 0)
				c = CHSV(p.hue + Data::getHue(), dis, constrain(p.alpha, 0, 255));
			else 
				c = gfx.getColour(p.hue, constrain(p.alpha * 1.5f, 0, 255));
			gfx.drawPointDepth(pos, c);

			if (p.isDead())
				return true;
			return false;
		});
		particles.erase(end, particles.end());

	}

	const static uint8_t numParticles = 64;
	
	std::vector<Particle> particles;

};


class ParticleSystemSpin : public ParticleSystem {
public: 
	Particle getParticle(bool side) {
		Particle p;
		float t;
		if (side)		t = theta;
		else			t = TWO_PI - theta;
		//lastSide = !lastSide;
#if 0
		p.pos.x = 2.2 * cos(t) * cos(Data::roll);
		p.pos.z = 2.2 * sin(t) * cos(Data::pitch);
		p.pos.y = sin(Data::pitch) * p.pos.z + sin(Data::roll) * p.pos.x;
#else
		p.pos.x = 2.2 * cos(t);
		p.pos.z = 2.2 * sin(t);
		p.pos.y = cos(t / 4);
#endif
		p.vel = Vec3::getRandom();
		p.vel *= 0.08;
		if ((p.pos + p.vel).Len() < GuiVars1) {
			p.vel = -p.vel;
		}
		p.acc = Vec3(0.0f, 0.0f, 0.0f);
		p.alpha = 255.0f;
		
		//if (side) {
			p.col = gfx.getColour(random8(30));
			p.hue = Data::getHue() + random8(30);
		//}
		//else {
		//	p.col = gfx.getColour(64+random8(30));
		//	p.hue = Data::getHue() +64+ random8(30);
		//}
		p.mass = 1.0f;
		return p;
	}

	void addParticle(bool side) {
		if (particles.size() == maxParticles)
			return;
		particles.push_back(getParticle(side));
	}

	virtual void decreaseLife(Particle &p) {
		p.alpha -= 3;
	}

	void incTheta(float d = 0.08) {
		theta += d;
		if (theta > 4*TWO_PI)
			theta -= 4*TWO_PI;
	}

	float theta = 0;


};

class PatternSpinningParticles : public _Pattern {
public:
	PatternSpinningParticles() : _Pattern("Spinning Particles") {
	}

	void start() {
		ps.init();
	}

	void stop() {
		particles.clear();
	}
	uint8_t drawFrame() {
		gfx.resetZ();
		if (!_Pattern::useCustomEffect) {
			gfx.fade(128);
		}
		ps.addParticle(true);
		ps.addParticle(false);
		ps.run();
		ps.incTheta();
		return 0;
	}
	

	ParticleSystemSpin ps;
};