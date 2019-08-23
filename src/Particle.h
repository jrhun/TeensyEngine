#pragma once

#include "Vec3.h"
#include "Color.h"

class Particle P {
	Particle() {}
	Particle(Particle &p) : pos(p.pos), vel(p.vel), acc(p.acc), alpha(p.alpha), col(p.col) {}

	void addForce(Vec3 f) {

	}

	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
	float mass; 
	float alpha;
	CRGB col;
};