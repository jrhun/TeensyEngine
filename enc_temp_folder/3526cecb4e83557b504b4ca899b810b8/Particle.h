#pragma once

#include "Vec3.h"
#include "Color.h"
#include <vector>

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
	Particle(Particle &p) : pos(p.pos), vel(p.vel), acc(p.acc), alpha(p.alpha), col(p.col) {}

	void applyForce(Vec3 &force) {
		acc += (force / mass);
	}

	void update() {
		vel += acc;
		pos += vel;
		acc *= 0;
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
	float mass = 0; 
	float alpha = 0;
	CRGB col = CRGB::Black;
};

constexpr int maxParticles = 500;
std::vector<Particle> particles;
//Particle particlesStorage[maxParticles];
//Particle *particles[maxParticles];
//int numParticles = 0;

class ParticleSystem {
public:
	ParticleSystem() {
		//if (!initalised) {
		//	initalised = true;
		//	init();
		//}
	}


	virtual Particle getParticle() {
		Particle p;
		p.pos = Vec3( 0.0f, 0.0f, 0.0f ); //{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 255.0f };
		p.pos.x = 2 * cos(theta);
		p.pos.z = 2 * sin(theta);
		p.vel = Vec3::getRandom() - Vec3(0.5f, 0.5f, 0.5f);
		p.vel *= 0.1;
		p.acc = Vec3(0.0f, 0.0f, 0.0f);
		p.alpha = 255.0f;
		p.col = gfx.getColour(random8(40));
		return p;
	}

	virtual void setParticle(Particle &p) {
		p.pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 255.0f };
		p.vel = Vec3::getRandom() - Vec3(0.5f, 0.5f, 0);
		p.acc = { 0.0f, 0.0f, 0.0f };
		p.alpha = 255.0f;
		p.col = gfx.getColour(random8(40));
	}

	float theta = 0;

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
		for (auto p : particles) {
			if (!p.isDead())
				p.applyForce(f);
		}
		//for (int i = 0; i < numParticles; i++) {
		//	particles[i]->applyForce(f);
		//}
	}

	virtual void decreaseLife(Particle &p) {
		p.alpha -= 2;
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

		auto end = std::remove_if(particles.begin(), particles.end(), [this](Particle &p) {
			// update particle
			p.update();
			
			// render particle
			Vec3 pos = p.pos;
			engine.sst.TransformSphere(pos);
			gfx.drawPointDepth(pos, p.col.nscale8_video(myMap(p.alpha, 0, 255, 64, 255)));

			// decrease alpha/life
			decreaseLife(p);

			if (p.isDead())
				return true;
			return false;
		});
		particles.erase(end, particles.end());

		//theta += 0.04;
		if (theta > TWO_PI)
			theta -= TWO_PI;

	}



	static void init() {
		//particles.reserve(maxParticles);
	}
	//static bool initalised;
	
};

//bool ParticleSystem::initalised = false;