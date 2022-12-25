#pragma once

#include "Vec3.h"
#include "Mat.h"
#include "Color.h"
#include <vector>

//https://www.bfilipek.com/2014/04/flexible-particle-system-container-2.html

class Particle {
public:
	Particle() {}
	//Particle() :
	//	pos(0.0f, 0.0f, 0.0f),
	//	vel(0.0f, 0.0f, 0.0f),
	//	acc(0.0f, 0.0f, 0.0f),
	//	mass(1.0f),
	//	alpha(0.0f),
	//	col(0, 0, 0)
	//{}
	Particle(const Particle &p) : pos(p.pos), vel(p.vel), acc(p.acc), alpha(p.alpha) { hue = p.hue; mass = p.mass; col = p.col; } // col(p.col), hue(p.hue), mass(p.mass)

	void applyForce(Vec3 &force) {
		acc += force;// (force / mass);
	}

	void update(bool limit = false) {
		vel += acc;
		pos += vel;
		acc *= 0;

		//limit vel
		
		if (limit) {
			if (pos.y >= 1.5f) {
			pos.y = 1.5f;
			//vel.y = 0;
			vel.y = -vel.y * 0.5;
			vel *= 0.9;
			}
		}

		//if (pos.y <= 0 || pos.y >= SCREEN_HEIGHT)
		//	alpha = 0;
		//if (pos.x < 0)
		//	pos.x += SCREEN_WIDTH;
		//if (pos.x > SCREEN_WIDTH)
		//	pos.x -= SCREEN_WIDTH;
	}

	bool isDead() {
		if (alpha <= 5) return true;
		return false;
	}

	Vec3 pos = { 0.0f,0.0f,0.0f };
	Vec3 vel = { 0.0f,0.0f,0.0f };
	Vec3 acc = { 0.0f,0.0f,0.0f };
	float mass = 1.0f; 
	float alpha = 0;
	CRGB col = CRGB::Black;
	uint8_t hue = 0;

	static float distance(Particle p1, Particle p2) {
		Vec3 d = p2.pos - p1.pos;
		float d1 = d * d;
		return sqrt(d1);
	}
};

constexpr int maxParticles = 500;
std::vector<Particle> particles;
//Particle particlesStorage[maxParticles];
//Particle *particles[maxParticles];
//int numParticles = 0;

class ParticleSystem {
public:
	ParticleSystem() : limit(false){
		//if (!initalised) {
		//	initalised = true;
		//	init();
		//}
	}


	virtual Particle getParticle() {
		Particle p;
		p.pos = Vec3( 0.0f, 0.0f, 0.0f ); //{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 255.0f };
		p.vel = Vec3::getRandom() - Vec3(0.5f, 0.5f, 0.5f);
		p.vel *= 0.1;
		p.acc = Vec3(0.0f, 0.0f, 0.0f);
		p.alpha = 255.0f;
		p.col = gfx.getColour(random8(40));
		p.hue = Data::getHue() + random8(40);
		p.mass = 1.0f;
		return p;
	}

	virtual void setParticle(Particle &p) {
		p.pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 255.0f };
		p.vel = Vec3::getRandom() - Vec3(0.5f, 0.5f, 0);
		p.acc = { 0.0f, 0.0f, 0.0f };
		p.alpha = 255.0f;
		p.col = gfx.getColour(random8(40));
		p.hue = Data::getHue() + random8(40);
	}


	//Particle * nextFreeParticle() {
	//	for (auto p : particlesStorage) {
	//		if (p.isDead())
	//			return &p;
	//	}
	//	return NULL;
	//}

	void addParticle() {
		if (particles.size() == maxParticles)
			return;
		particles.push_back(getParticle());
		//if (numParticles < maxParticles) {
		//	Particle *p = nextFreeParticle();
		//	if (p) {
		//		setParticle(*p);
		//		particles[numParticles] = p;
		//		numParticles++;
		//	}
		//	
		//}
	}

	void applyForce(Vec3 f) {
		for (unsigned int i = 0; i < particles.size(); i++) {
			particles[i].applyForce(f);
		}
		//for (auto p : particles) {
			//if (!p.isDead())
			//p.acc += f;// .applyForce(f);
		//}
		//for (int i = 0; i < numParticles; i++) {
		//	particles[i]->applyForce(f);
		//}
	}

	virtual void decreaseLife(Particle &p) {
		p.alpha -= 1;
	}

	void run() {
		//for (auto iter = particles.begin(); iter != particles.end(); ) {
		//	Particle *p = &(*iter);
		//	if (p->isDead()) {
		//		iter = particles.erase(iter);
		//	}
		//	else {
		//		p->update();
		//		p->render();
		//		iter++;
		//	}
		//}
		//alt using remove if

		//for (int i = numParticles-1; i >= 0 ; i--) {
		//	Particle &p = *particles[i];
		//	p.update();
		//	gfx.drawPointDepth(p.pos, p.col);
		//	decreaseLife(p);
		//	if (p.isDead()) {
		//		// move to end
		//		particles[i] = particles[numParticles - 1];
		//		particles[numParticles - 1] = NULL;
		//		numParticles--;
		//	}
		//}

		static float rotateX = 0, rotateY = 0;
		Mat3 m = Mat3::RotationY(rotateX) * Mat3::RotationY(rotateY);
		rotateX += (GuiVars4 - 1) * 0.1;
		rotateY += (GuiVars5 - 1) * 0.1;
		if (rotateX >  TWO_PI)
			rotateX -= TWO_PI;
		if (rotateX < -TWO_PI)
			rotateX += TWO_PI;
		if (rotateY >  TWO_PI)
			rotateY -= TWO_PI;
		if (rotateY < -TWO_PI)
			rotateY += TWO_PI;
		auto end = std::remove_if(particles.begin(), particles.end(), [this, m](Particle &p) {
			// update particle
			p.update(limit);
			
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
			//gfx.drawPointDepth(pos, p.col.nscale8_video(myMap(constrain(p.alpha, 0, 255), 0, 255, 64, 255)));

			// decrease alpha/life
			decreaseLife(p);

			if (p.isDead())
				return true;
			return false;
		});
		particles.erase(end, particles.end());

	}


	bool limit;


	static void init() {
		//particles.reserve(maxParticles);
	}
	//static bool initalised;
	
};

//bool ParticleSystem::initalised = false;

