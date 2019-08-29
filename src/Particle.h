#pragma once

#include "Vec3.h"
#include "Color.h"
#include <vector>

class Particle {
public:
	Particle() :
		pos(0.0f, 0.0f, 0.0f),
		vel(0.0f, 0.0f, 0.0f),
		acc(0.0f, 0.0f, 0.0f),
		mass(1.0f),
		alpha(0.0f),
		col(0, 0, 0)
	{}
	Particle(Particle &p) : pos(p.pos), vel(p.vel), acc(p.acc), alpha(p.alpha), col(p.col) {}

	void applyForce(Vec3 &force) {
		acc += (force / mass);
	}

	void update() {
		vel += acc;
		pos += vel;
		acc *= 0;
	}

	bool isDead() {
		if (alpha == 0) return true;
		return false;
	}

	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
	float mass; 
	float alpha;
	CRGB col;
};

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
		p.pos = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 255.0f };
		p.vel = Vec3::getRandom();
		p.acc = { 0.0f, 0.0f, 0.0f };
		p.alpha = 255.0f;
		p.col = gfx.getColour(random8(40));
		return p;
	}

	void addParticle() {
		if (particles.size() == maxParticles)
			return;
		particles.push_back(getParticle());
	}

	void applyForce(Vec3 f) {
		for (auto p : particles) {
			if (!p.isDead())
				p.applyForce(f);
		}
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
		auto end = std::remove_if(particles.begin(), particles.end(), [this](Particle &p) {
			if (p.isDead())
				return true;
			// update particle
			p.update();
			
			// render particle
			gfx.drawPointDepth(p.pos, p.col);

			// decrease alpha/life
			decreaseLife(p);
			return false;
		});
		particles.erase(end, particles.end());

	}



	static void init() {
		particles.reserve(maxParticles);
	}
	//static bool initalised;
	static std::vector<Particle> particles;
	static const int maxParticles = 250;
};

//bool ParticleSystem::initalised = false;