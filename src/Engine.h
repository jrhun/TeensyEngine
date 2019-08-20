#pragma once

//Main file for controlling UI, drawing, etc

// Includes
#if defined(ESP32) || defined(CORE_TEENSY)
//  #if defined(__IMXRT1052__) || defined(__IMXRT1062__) TEENSY 4.X
#include "GraphicsFastLED.h"
#else // PC includes
#include "GraphicsPC.h"
#endif // endif includes

#include "ScreenTransform.h"
#include "Cube.h"


class Engine {
public:
	Engine(Graphics *gfx) : gfx(gfx) {
		sst.width = SCREEN_WIDTH;
		sst.height = SCREEN_HEIGHT;
	}




	void updateModel() {
		incAngle();
		// set each objects translation matrix to rotate seprately
		// 
	}

	Cube cube;

	void composeFrame() {
		// pipeline!
		//for each object
		//generate list of vertex
		//send to pipeline
		auto lines = cube.GetLines();
		for (auto &v : lines.vertices) {
			v.pos.z += 1.0f;
			sst.Transform(v.pos);
		}
	}

	void drawTest() {
		gfx->resetZ();
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				Vec3 a(i, i, 0);
				Vec3 b(i + 30, i, 255);
				gfx->DrawLineDepth(a, b, (i / 20.0) * 255, 255 - (i / 20.0) * 255, 50);
				//gfx->putPixel(i, i+j, (i / 20.0) * (j / 20.0) * 255);
			}
		}
	}

	void init() {
		gfx->init();
	}

	void clear() {
		gfx->clear();
	}

	void fade(unsigned char a) {
		gfx->fade(a);
	}

	void show() {
		gfx->show();
	}

	float anglex = 0;
	float angley = 0;

	void incAngle(float i = 0.01) {
		anglex += i;
		angley += i;
		if (anglex >= TWO_PI)
			anglex -= TWO_PI;
		if (angley >= TWO_PI)
			angley -= TWO_PI;
	}

	Graphics * gfx;
	ScreenTransform sst;
};


