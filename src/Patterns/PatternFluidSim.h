#pragma once

#include "Pattern.h"
#include "../../data.h"
#include "../Vec2.h"

//sample running http://neuroid.co.uk/lab/fluid/
//js http://neuroid.co.uk/lab/fluid/fluid.js
//processing https://github.com/weymouth/lily-pad
// forum talking about latice boltman calculations https://github.com/CodingTrain/Rainbow-Topics/issues/178

// use int16 with step of 256 for finer movements but still avoiding floats 
//only one particle in each spot, no blending for water/sand


class _Particle {
public:
	_Particle() {}
	_Particle(const _Particle& rhs) : pos(rhs.pos), vel(rhs.vel), acc(rhs.acc), hue(rhs.hue) {}
	_Particle& operator=(const _Particle& rhs) {
		pos = rhs.pos; 
		vel = rhs.vel; 
		acc = rhs.acc;
		hue = rhs.hue; 
		return *this;
	}

	Vec2 pos;
	Vec2 vel;
	Vec2 acc;

	uint8_t hue;
};

class PatternLavaLamp : public _Pattern {
public: PatternLavaLamp() : _Pattern("Lava Lamp") {}

	  class _LavaParticle : public _Particle {
	  public:
		  //heat of particle
		  //radius of particle
		  float temp;

	  };

	  void start() {
		  for (int i = 0; i < 100; i++) {
			  _LavaParticle p;
			  p.pos = Vec2(random8(0, SCREEN_WIDTH), random8(SCREEN_HEIGHT - 8, SCREEN_HEIGHT));
			  p.vel = Vec2(0, 0);
			  p.acc = Vec2(0, 0);
			  p.temp = 10; 
			  p.hue = random8(32);
			  parts.push_back(p);

		  }
	  }

	  void stop() {
		  parts.clear(); 
	  }

	  _Particle p; 

	  //set up gradient from top to bottom of temperature
	  //particles gain temperature at the bottom and diffuse that temperature out
	  //bouyancy according to delta T 
	  //particles need aspect of surface tension to hold together 
	  // can provide gentle forces using perlin noise 

	  //between each particle pair
	  // spring particles apart if distance between them less than 2x radius of particles
	  //if distance greater than 2x radius but less than gravity radius, attract particles together (estimate surface tension?)
	  //transfer heat from hotter particle to less hot one, rate 0.045

	  //for each particle
	  //bounds check - if pos less than particle spring backwards

//apply forces

//apply heat zone at bottom 0.2

//cooling zone at top 0.2

//heat loss 0.012


uint8_t drawFrame() {
	float dt = 0.1;
	//Vec3 k = Vec3(Data::ax, Data::ay, Data::az);		  
	//p.acc += projectOntoPlane(k, p.pos.x, p.pos.y);


	float L;
	float K = 3; //spring constant 
	float M = 10;

	float R = 1.2; //particle radius 
	float G = 0.05;
	float GG = 1.5; //particle gravity force
	float GGrad = 2.5; //particle gravity radius
	float P = 0.03; //sensitivity to temp change
	float H = 0.535; //sensitivity to velocity

	float heatRate = 0.3;
	float heatTransferRate = 0.010;
	float healLoss = 0.02;
	float coolRate = 0.1;

	//P = myMap(GuiVars1, 0, 2, 0.025, 0.1); //temp sens
	//H = myMap(GuiVars2, 0, 2, 0.1, 1.0); //vel sens
	//GG = myMap(GuiVars3, 0, 2, 0.1, 2); //particle gravity

	//heatTransferRate = myMap(GuiVars4, 0, 2, 0.001, 0.02);
	//healLoss = myMap(GuiVars5, 0, 2, 0.005, 0.05);


	//GGrad = myMap(GuiVars6, 0, 2, 3, 15);//particle gravity radius 
	float f = 0.3;// myMap(GuiVars6, 0, 2, 0.1, 0.5); //random velocity amount
	static uint8_t xOffset = 0;
	static uint8_t xCount = 0;
	if (++xCount == 15) {
		xCount = 0;
		xOffset = (xOffset + 1) % SCREEN_WIDTH;
	}
	//apply forces
	Vec2 dif;
	for (auto& p : parts) {
		for (auto& p2 : parts) {
			dif = p.pos - p2.pos;
			//wrapping
			if (dif.x > SCREEN_WIDTH / 2) {
				dif.x -= SCREEN_WIDTH;
			}
			else if (dif.x < -SCREEN_WIDTH / 2) {
				dif.x += SCREEN_WIDTH;
			}
			L = dif.Len();
			if (L == 0) continue;
			dif.Normalize();
			//spring particles apart if too close
			if (L < R * 2) {
				float a = (K * (2 * R - L));
				p.acc += dif * a;
				p2.acc -= dif * a;

				//heat transfer if touching 
				if (p.temp < p2.temp) {
					p.temp += heatTransferRate;
					p2.temp -= heatTransferRate;
				}
				else {
					p.temp -= heatTransferRate;
					p2.temp += heatTransferRate;
				}
			}
			else if (L < R * GGrad) {
				float a = GG / (L * L);
				p.acc -= dif * a;
				p2.acc += dif * a;
			}

		}


		if (p.pos.x < R)					p.acc.x += (K * (R - p.pos.x));
		if (p.pos.x > SCREEN_WIDTH - R)	p.acc.x -= (K * (p.pos.x - SCREEN_WIDTH + R));
		if (p.pos.y < R)					p.acc.y += (K * (R - p.pos.y));
		if (p.pos.y > SCREEN_HEIGHT - R)	p.acc.y -= (K * (p.pos.y - SCREEN_HEIGHT + R));

		p.acc += projectOntoPlane(Vec3(Data::ax, Data::ay, Data::az), p.pos.x, p.pos.y) / 5;

		p.acc.y += -(P * p.temp);
		if (random8(20) == 0) {
			p.vel.x += myMap(random8(), 0, 255, -f, f); //-0.5
		}

		if (p.pos.y > SCREEN_HEIGHT - 10) {
			if (xOffset > SCREEN_WIDTH - 16) {
				if (p.pos.x > xOffset or (p.pos.x < (xOffset + 16 - SCREEN_WIDTH))) {
					p.temp += heatRate;
				}
			}
			else {
				if ((p.pos.x > (xOffset)) and (p.pos.x < (xOffset + 16))) {
					p.temp += heatRate;
				}
			}

		}
		if (p.pos.y < 10) {
			p.temp -= coolRate;
		}
	}
		  //move and draw
		gfx.clear();
		for (auto& p : parts) {
			  
			p.vel = (p.vel + p.acc / M)* H;
			p.acc *= 0; 
			//limit velocity
			float v = p.vel.Len(); 
			if (v > 0.8) {
				p.vel *= 0.8 / v; 
			}
			else if (v < 0.05) {
				p.temp -= healLoss;
			}
			p.pos += p.vel; 
			CRGB c; 
			if (Data::paletteIndex == 0) {
				uint8_t hue = 0; // myMap(p.temp, -30, 30, 0, 64, true);
				hue += uint16_t((p.pos.x) + (p.pos.y) +  Data::getHue()) % 255;
				c = CHSV( hue, myMap(p.temp, -10, 30,  64, 255, true), 255);
			}
			else {
				c = gfx.getColour(myMap(p.temp, -30, 30, 0, 255, true) );
			}
			//if (p.temp > 50) {
				// c = CRGB(255, 255, 0); 
			//}
			//else if (p.temp > 30) {
				// c = CRGB(255, myMap(p.temp, 50, 30, 255, 0), 0); 
			//}
			//else if (p.temp > 20) {
				// c = CRGB(255, 0, myMap(p.temp, 30, 20, 0, 255));
			//}
			//else if (p.temp > -20) {
				// c = CRGB(myMap(p.temp, 20, -20, 255, 0), 0, 255);
			//}
			//else if (p.temp > -40) {// < 100
				// c = CRGB(0, 0, myMap(p.temp, -20, -40, 255, 100));
			//}
			//else {
				// c = CRGB(0, 0, 100);
			//}
			//CRGB c = CHSV(myMap(p.temp, -120, 120, 120, 0, true), 255, 255);
			uint8_t rr = 1; 
			for (int8_t i = -rr; i <= rr; i++) {
				for (int8_t j = -rr; j <= rr; j++) {
					  
					gfx.blendPixel(int8_t(p.pos.x + i + SCREEN_WIDTH) % SCREEN_WIDTH, p.pos.y + j, c, 128);
				}
			}
			//gfx.putPixel(p.pos.x	, p.pos.y, c); 
			//gfx.blendPixel(p.pos.x + 1, p.pos.y	 , c, 128);
			//gfx.blendPixel(p.pos.x	, p.pos.y + 1, c, 128);
			//gfx.blendPixel(p.pos.x - 1, p.pos.y	 , c, 128);
			//gfx.blendPixel(p.pos.x	, p.pos.y - 1, c, 128);

		}
		gfx.blur(170);	  
		//gfx.drawLine(xOffset, SCREEN_HEIGHT - 1, xOffset + 16, SCREEN_HEIGHT - 1, CHSV(0, 129, 255));
		  
		return returnVal;
	}

	  void ensureWithinBounds(_Particle& x) {
		  if (x.pos.y < 0) {
			  x.pos.y = 0; 
			  x.vel.y /= -2;
		  }
		  else if (x.pos.y > SCREEN_HEIGHT - 1) {
			  x.pos.y = SCREEN_HEIGHT - 1; 
			  x.vel.y /= -2; 
		  }
		  if (x.pos.x >= SCREEN_WIDTH) {
			  x.pos.x -= SCREEN_WIDTH;
		  }
		  else if (x.pos.x < 0) {
			  x.pos.x += SCREEN_WIDTH;
		  }
	  }
	  Vec2 projectOntoPlane(Vec3 force, int8_t x, int8_t y = -1) {
		  //project a force (i.e from accelerometer) onto the sphere on a plane tangential to the sphere at a given x/y coordinates
		  // if  y isn't provided we use a cylinder instead of sphere
		  float theta = myMap(x, 0, SCREEN_WIDTH, 0, 2 * PI);
		  float phi = myMap(y, 0, SCREEN_HEIGHT, PI / 8, 7 * PI / 8); 
		  Vec3 yVec;
		  if (y == -1) {
			  yVec = Vec3(0, 1, 0); // pointing up -> modeling cylinder
		  }
		  else {
			  //attempting to model sphere, phi angle from y axis, theta angle from x axis towards z axis
			  yVec = Vec3(cos(theta) * sin(phi), cos(phi), sin(theta) * sin(phi));
			  yVec.Normalize(); //yvec is pointing directly out
		  }
		  Vec3 xVec = Vec3(cos(theta), 0, sin(theta)); //set phi to PI/2, i.e y axis = 0
		  xVec = Vec3(0,1,0) % xVec; //xVec is pointing out, cross with up to get x-axis, can do this as not dependent on y axis
		  yVec = yVec % xVec; // cross with x axis to get tangential in up direction i.e y axis
		  yVec.Normalize();
		  xVec.Normalize();
		  return Vec2(force * xVec, force * yVec); //dot product force with each axis
	  }

	  static const uint16_t nParticles = 200;
	  std::vector<_LavaParticle> parts;

};


class PatternSandSim : public _Pattern {
public:
	PatternSandSim() : _Pattern("Sand Sim") {

	}

	void start() {
		parts.clear(); 
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
			for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
				_grid.set(x, y, NULL);
			}
		}


		//parts.reserve(16 * 8);
		for (uint8_t i = 0; i < 16; i++) {
			for (uint8_t j = 0; j < 8; j++) {
				if (parts.size() > 500) return; 
				_Particle p;
				p.pos = Vec2(SCREEN_WIDTH / 2 + i - 8, 16 + j - 4);
				//p.pos = Vec2(SCREEN_WIDTH / 2 , 16 );
				p.vel = Vec2(0, 0);
				p.hue = random8(20);
				if (_grid.at(p.pos.x, p.pos.y) == NULL) {
					parts.push_back(p);
					_grid.set(p.pos.x, p.pos.y, &parts.back());
				}
			}
		}
	}

	void stop() {
		parts.clear(); 
	}

	uint8_t drawFrame() {

		float dt = 0.05;
		//advect/move particles
		moveParticles(dt);

		//convert to grid

		//update velocity/add force
		//float dir = myMap(GuiVars1, 0, 2, 0, 2 * PI);
		//float posX, posY, posZ;
		//posX = cos(dir);
		//posZ = sin(dir);
		//posY = myMap(GuiVars2, 0, 2, -5, 5);
		//Data::ax = myMap(GuiVars1, 0, 2, -5, 5);
		//Data::ay = myMap(GuiVars2, 0, 2, -5, 5);
		//Data::az = myMap(GuiVars3, 0, 2, -5, 5);
		//float vel = myMap(GuiVars3, 0, 2, 0.1, 2);

		//Vec2 f;

		//for (_Particle& p : parts) {
		//	f = getAccel(p.pos.x);
		//	p.vel *= 0.95; //dampen slightly
		//	p.vel += f * vel;
		//	
		//}

		//enforce boundary

		//solve pressures/relaxation
		for (_Particle& p : parts) {
			
		}

		//grid back to particles 

		//draw particles
		gfx.clear();
		for (_Particle& p : parts) {
			gfx.putPixel(p.pos.x, p.pos.y, gfx.getColour(p.hue));
		}
		
		return returnVal;
	}

	void moveParticles(float dt) {
		float vel = myMap(GuiVars3, 0, 2, 0.1, 2);
		for (_Particle& p : parts) {
			_Particle newp = p;

			Vec3 k = Vec3(Data::ax, Data::ay, Data::az);
			Vec2 f = projectOntoPlane(k, p.pos.x, p.pos.y);
			p.vel *= 0.95;
			p.vel += f * vel;
			

			newp.pos = p.pos + p.vel * dt;
			_constrain(newp); //constrain to boundry and bounce 
			Vec2 dx = newp.pos - p.pos;
			int16_t xi1, yi1, xi2, yi2;
			xi1 = p.pos.x;
			yi1 = p.pos.y;
			xi2 = newp.pos.x;
			yi2 = newp.pos.y;
			int8_t dxi = xi2 - xi1;
			int8_t dyi = yi2 - yi1;
			if (abs(dxi) > 0 || abs(dyi) > 0) {
				// have moved to new cell, and spot is free
				if (_grid.at(xi2, yi2) == NULL) {//and spot is free
					_grid.set(xi1, yi1, NULL);
					p = newp;
					_grid.set(xi2, yi2, &p);
				}
				else {
					//spot is taken, check along velocity vector 
					do {
						dxi = xi2 - xi1;
						dyi = yi2 - yi1;
						
						if (abs(dx.x) > abs(dx.y)) {
							if (abs(dyi) > 0) {
								newp.pos.y += (dyi > 0) ? -1 : 1; //work backwards along y 
							}
							else {
								newp.pos.x += (dxi > 0) ? -1 : 1;
							}
						}
						else {
							if (abs(dxi) > 0) {
								newp.pos.x += (dxi  > 0) ? -1 : 1; 
							}
							else {
								newp.pos.y += (dyi > 0) ? -1 : 1;
							}
						}
						if (_grid.at(newp.pos.x, newp.pos.y) == NULL) {
							// add some velocity to particle we hit
							//to do
							//loose some velocity from bouncing
							p.vel *= 0.5; 
							_grid.set(p.pos.x, p.pos.y, NULL);
							//newp.vel.y = newp.vel.y * (newp.pos.y - p.pos.y) * 0.5; //add change to velocity as bounce

							p = newp;
							_grid.set(p.pos.x, p.pos.y, &p);

						}
						xi2 = newp.pos.x;
						yi2 = newp.pos.y;
					} while (xi1 != xi2 && yi1 != yi2);
					//if we get here we've either set a new post or couldn't move
				}
			}
			else {
				p = newp;
				//now move water to the sides
			}
			
		}
	}

	void _constrain(_Particle& p) {
		//constrain vector to boundary
		Vec2& v = p.pos; 
		while (v.x > SCREEN_WIDTH) {
			v.x -= SCREEN_WIDTH; 
			p.vel.x *= 0.5;
		}
		while (v.x < 0) {
			v.x += SCREEN_WIDTH; 
			p.vel.x *= 0.5;
		}
		if (v.y < 0) {
			v.y = 0; 
			p.vel.y *= -0.25; //bounce and dampen

		}
		else if (v.y > SCREEN_HEIGHT - 6) {
			v.y = SCREEN_HEIGHT - 6;
			p.vel.y *= -0.25;
		}
	}



	Vec2 getAccel(int8_t x, int8_t y = -1) {
		//https://stackoverflow.com/questions/23472048/projecting-3d-points-to-2d-plane

		// for other purusal https://math.stackexchange.com/questions/2586792/3d-point-projection-on-2d-plane-using-normal-vector-and-projection-vector
		//accelerometer gives us a vector in 3d for current acceleration 
		//as we have a sphere, we want to convert this to an ax and ay for each point around the sphere
		//we can do this by finding a plane for each point around the sphere -> simplified to a cylinder i.e ignoring height on sphere, find plane tangential to cylinder
		//define the x and y directions for this plane as unit vectors
		//then project the acceleration K onto the X and Y onto the unit vectors 
		//taking the magnitude of these gives the x and y coordinates i.e. the transformed velocity 
		Vec3 k = Vec3(Data::ax, Data::ay, Data::az); 
		float theta = myMap(x, 0, SCREEN_WIDTH, 0, 2 * PI); 
		Vec3 yVec = Vec3(0, 1, 0); // pointing up
		Vec3 xVec = Vec3(cos(theta), 0, sin(theta)); // cos(theta) + sin(theta) convinently is unit vector
		//xVec point straight out from circle currently, cross product with yVec gives the proper vector tangential with circle/cylinder
		xVec = yVec % xVec;
		xVec.Normalize();
		//proj K onto xVec ( dot product of K * X / mag(X) * X
		float fx, fy;
		fx = k * xVec; 
		fy = k * yVec;
		return Vec2(fx, fy);		
	}

	Vec2 projectOntoPlane(Vec3 force, int8_t x, int8_t y = -1) {
		float theta = myMap(x, 0, SCREEN_WIDTH, 0, 2 * PI);
		float phi = myMap(y, 0, SCREEN_HEIGHT, PI / 8, 7 * PI / 8);
		Vec3 yVec;
		if (y == -1) {
			yVec = Vec3(0, 1, 0); // pointing up -> currently modeling cylinder
		}
		else {
			//attempting to model sphere, phi angle from y axis, theta angle from x axis towards z axis
			yVec = Vec3(cos(theta) * sin(phi), cos(phi), sin(theta) * sin(phi));
			yVec.Normalize(); //yvec is pointing directly out
		}
		Vec3 xVec = Vec3(cos(theta), 0, sin(theta)); //set phi to PI/2, i.e y axis = 0
		xVec = Vec3(0, 1, 0) % xVec; //xVec is pointing out, cross with up to get x-axis, can do this as not dependent on y axis
		yVec = yVec % xVec; // cross with x axis to get tangential in up direction i.e y axis
		yVec.Normalize();
		xVec.Normalize();
		return Vec2(force * xVec, force * yVec); //dot product force with each axis
	}



	struct Grid {
		_Particle* gridData[SCREEN_WIDTH][SCREEN_HEIGHT];

		_Particle* at(uint8_t x, uint8_t y) {
			if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
				return gridData[x][y];
			else
				return NULL;
		}

		void set(uint8_t x, uint8_t y, _Particle* p) {
			if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
				if (p != NULL) {
					gridData[x][y] = p;
				}
				else {
					gridData[x][y] = NULL;
				}
			}
				
		}
	} _grid;




	std::vector<_Particle> parts;
};
	


//based on https://github.com/mike-rankin/ESP32_CoinCell_Color_TFT/blob/master/Code/colorcoin_test/colorcoin_test.ino
//also dislike this method... 
struct Grain {
	Grain() {}

	int16_t px, py;
	int16_t vx, vy;

	uint8_t hue;
};

class PatternFluidSimSand : public _Pattern {
public:
	PatternFluidSimSand() : _Pattern("Fluid Sim") {
		clear();
	}

	void clear() {
		for (uint16_t i = 0; i < 256; i++) {
			bitField[i] = 0;
		}
		for (uint16_t i = 0; i < NUM_GRAINS; i++) {
			grains[i].px = grains[i].py = grains[i].vx = grains[i].vy = 0;
			grains[i].hue = 0;
		}
	}

	void start() {
		clear();
		int16_t i, j, x, y; 
		for (i = 0; i < NUM_GRAINS; i++) {
			do {
				grains[i].px = random16(SCREEN_WIDTH * 256);
				grains[i].py = random16(SCREEN_HEIGHT* 256);
				for (j = 0; (j < i) && (((grains[i].px / 256) != (grains[j].px / 256)) ||
					((grains[i].py / 256) != (grains[j].py / 256))); j++);
			} while (j < i); // Keep retrying until a clear spot is found
			x = grains[i].px / 256;
			y = grains[i].py / 256;
			grains[i].vx = grains[i].vy = 0; 
			grains[i].hue = random8(32);
			markPos(x, y, true);
			
		}
	}


	uint8_t drawFrame() {

		float dir = myMap(GuiVars1, 0, 2, 0, 2 * PI); 
		float posX, posY; 
		posX = cos(dir); 
		posY = sin(dir); 
		float vel = myMap(GuiVars2, 0, 2, 10, 250);


		for (uint16_t i = 0; i < NUM_GRAINS; i++) {
			int16_t vx, vy, newx, newy;
			int16_t x1, y1, x2, y2;
			int32_t v2 = 0;
			vx = grains[i].vx + posX * vel; //base 9.8ms
			vy = grains[i].vy + posY * vel;
			v2 = (int32_t)(vx * vx) + (int32_t)(vy * vy);
			//if too big trim it
			if (v2 > 400000) {
				vx = (vx * 5) / 8;
				vy = (vy * 5) / 8;
			}
			newx = grains[i].px + vx;
			newy = grains[i].py + vy;
			if (newx > SCREEN_WIDTH * 256) {
				newx = (SCREEN_WIDTH - 1) * 256;
				vx /= -2;
			}
			else if (newx < 0) {
				newx = 0; 
				vx /= -2;
			}
			//newx %= SCREEN_WIDTH * 256;
			if (newy > SCREEN_HEIGHT * 256) {
				newy = (SCREEN_HEIGHT - 1) * 256;
				vy /= -2;
			}
			else if (newy < 0) {
				newy = 0;
				vy /= -2;
			}
			x1 = grains[i].px;
			x1 /= 256;
			y1 = (grains[i].py);
			y1 /= 256;
			x2 = newx / 256;
			y2 = newy / 256;
			if (x1 != x2 || y1 != y2 && checkPosTaken(x2, y2)) { //moving to new position but occupied
				if (abs(vx) > abs(vy)) { //x axis faster
					y2 = y1; //
					if (checkPosTaken(x2, y2) == false) {
						newy = grains[i].py;
						vy = (vy / -2) + random8(8);         // and bounce Y velocity
					}
					else { // X pixel is taken, so try Y...
						y2 = newy / 256; x2 = x1;
						if (checkPosTaken(x2, y2) == false) { // Pixel is free, take it, but first...
							newx = grains[i].px; // Cancel X motion
							vx = (vx / -2) + random8(8);         // and bounce X velocity
						}
						else { // Both spots are occupied
							newx = grains[i].px; // Cancel X & Y motion
							newy = grains[i].py;
							vx = (vx / -2) + random8(8);         // Bounce X & Y velocity
							vy = (vy / -2) + random8(8);
						}
					}
				}
				else { //y axis faster 
					x2 = x1;
					if (checkPosTaken(x2, y2) == false) { // Pixel's free!  Take it!  But...
						newx = grains[i].px; // Cancel X motion
						vx = (vx / -2) + random8(8);        // and bounce X velocity
					}
					else { // Y pixel is taken, so try X...
						x2 = newx / 256; y2 = y1;
						if (checkPosTaken(x2, y2) == false) { // Pixel is free, take it, but first...
							newy = grains[i].py; // Cancel Y motion
							vy = (vy / -2) + random8(8);        // and bounce Y velocity
						}
						else { // Both spots are occupied
							newx = grains[i].px; // Cancel X & Y motion
							newy = grains[i].py;
							vx = (vx / -2) + random8(8);         // Bounce X & Y velocity
							vy = (vy / -2) + random8(8);
						}
					}
				}
			}
			grains[i].px = newx; // Update grain position
			grains[i].py = newy; // possibly only a fractional change
			grains[i].vx = vx;
			grains[i].vy = vy;
			y2 = newy / 256; x2 = newx / 256;
			if (x1 != x2 || y1 != y2)
			{
				markPos(x1, y1, false); // erase old pixel
				markPos(x2, y2, true); // Set new pixel
			}

		}

		gfx.clear();
		for (uint16_t i = 0; i < NUM_GRAINS; i++) {
			gfx.putPixel(grains[i].px / 256, grains[i].py / 256, gfx.getColour(grains[i].hue));
		}



		return returnVal;
	}

	uint8_t bitField[256]; //SCREEN_WIDTH * SCREEN_HEIGHT / 8
	bool checkPosTaken(int16_t x, int16_t y) {
		uint16_t i = x + y * SCREEN_WIDTH;
		uint8_t n = i % 8; //what bit we want
		i /= 8; //what byte
		bool taken = (bitField[i] >> n) & uint8_t(1);
		return taken; 
	}
	void markPos(int16_t x, int16_t y, bool set) {
		uint16_t i = x + y * SCREEN_WIDTH; 
		uint8_t n = i % 8; //what bit we want
		i /= 8; //what byte
		if (set) {
			bitField[i] |= uint8_t(1) << n; //set bit
		}
		else {
			bitField[i] &= ~(uint8_t(1) << n); //clear bit
		}
	}



	static const uint16_t NUM_GRAINS = 8*64; 
	Grain grains[NUM_GRAINS];
};

//try easy way first, no particles, use the LED buffer 
//noita method https://www.youtube.com/watch?v=prXuyMCgbTc
// i don't like hoow this looks, and not sure how to add in impulse force
class PatternFluidSim2 : public _Pattern {
public:
	PatternFluidSim2() : _Pattern("Fluid Sim") {}

	void start() {
		for (uint16_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
			buffer[i] = 0;
		}
	}

	uint8_t drawFrame() {
		gfx.clear();

		//add particle
		static uint8_t counter = 0;
		if (++counter == 5) {
			counter = 0;
			point(SCREEN_WIDTH / 2, 5) = random8();
		}

		//bottom up, move falling ones out the way first
		static uint8_t nextUpdate = 0;
		if (++nextUpdate == 10) {
			nextUpdate = 0;
			for (int8_t y = SCREEN_HEIGHT - 2; y >= 0; y--) {
				for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
					if (point(x, y) != 0) {
						if (point(x, y + 1) == 0) {
							point(x, y + 1) = point(x, y);
							point(x, y) = 0;
						}
						else {
							if (random8() > 150) {
								if (point((x - 1) + SCREEN_WIDTH, y + 1) == 0) {
									point((x - 1) + SCREEN_WIDTH, y + 1) = point(x, y);
									point(x, y) = 0;
								}
								else if (point(x + 1, y + 1) == 0) {
									point(x + 1, y + 1) = point(x, y);
									point(x, y) = 0;
								}
								else if (point((x - 1) + SCREEN_WIDTH, y) == 0) {
									point((x - 1) + SCREEN_WIDTH, y) = point(x, y);
									point(x, y) = 0;
								}
								else if (point(x + 1, y) == 0) {
									point(x + 1, y) = point(x, y);
									point(x, y) = 0;
								}
							}
							else {
								if (point(x + 1, y + 1) == 0) {
									point(x + 1, y + 1) = point(x, y);
									point(x, y) = 0;
								}
								else if (point((x - 1) + SCREEN_WIDTH, y + 1) == 0) {
									point((x - 1) + SCREEN_WIDTH, y + 1) = point(x, y);
									point(x, y) = 0;
								}
								else if (point(x + 1, y) == 0) {
									point(x + 1, y) = point(x, y);
									point(x, y) = 0;
								}
								else if (point((x - 1) + SCREEN_WIDTH, y) == 0) {
									point((x - 1) + SCREEN_WIDTH, y) = point(x, y);
									point(x, y) = 0;
								}

							}
						}
					}
				}
			}
		}


		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
			for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
				uint8_t v = point(x, y);
				if (v) {
					uint8_t b = v % 0b1111; 
					b = myMap(b, 0, 15, 196, 255, true);
					CRGB c = CHSV(140 + (v >> 4) * 2, 255, b); // only sets a colour if buffer[i] isn't 0
					gfx.putPixel(x, y, c);
				}

			}
		}
		return returnVal;
	}


	//void drawParticle(_Particle* p) {
	//	gfx.putPixel(p->px / STEP, p->px / STEP, gfx.getColour(p->hue));
	//}
	//const int16_t STEP = 256;

	uint8_t& point(uint8_t x, uint8_t y) {
		return buffer[_XY(x, y)];
	}

	uint16_t _XY(uint8_t x, uint8_t y) {
		x %= SCREEN_WIDTH;
		if (y < SCREEN_HEIGHT and y >= 0)
			return (x + y * SCREEN_WIDTH);
		else return 0;
	}

	uint8_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT]; //use bitwise check vs just use as val
};

#define IX(i,j) ((i) + ((N) * (j)))

class PatternFluidSim3 : public _Pattern {
public:
	PatternFluidSim3() : _Pattern("Fluid Sim") {}

	uint8_t drawFrame() {




		return returnVal;
	}

	void add_source(size_t n, float* x, float* s, float dt) {
		for (int i = 0; i < n; i++) {

			x[i] += dt * s[i]; 
		}
	}

	void advect() {

	}
	void diffuse() {

	}



	//arrays
	static const size_t N = SCREEN_WIDTH * SCREEN_HEIGHT; 
	Vec2 vel[N], vel_prev[N]; 
	float dens[N], dens_prev[N]; 
};