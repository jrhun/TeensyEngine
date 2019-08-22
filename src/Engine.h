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
#include "Mat.h"
#include "Vec3.h"
#include "Pipeline.h"
#include "Geometry.h"
#include "mathHelpers.h"


class Engine {
public:
	//typedef Pipeline<Vertex> Pipeline;
public:
	Engine(Graphics &gfx) :
		gfx(gfx),
		sst(SCREEN_WIDTH, SCREEN_HEIGHT),
		pipeline(gfx, sst) {}


	float cameraOffset = 2.0f;
	float offsetDir = 1.0f;

	void updateModel() {
		incAngle();
		if (cameraOffset > 4)
			offsetDir = -1.0f;
		if (cameraOffset < 2)
			offsetDir = 1.0f;
		cameraOffset += 0.01 * offsetDir;
		// set each objects translation matrix to rotate seprately
		// 
	}

	Cube cube;

	void composeFrame() {
		// pipeline!

		gfx.resetZ();
		//for each object
		//generate list of vertex
		//send to pipeline


		//for (int i = 0; i < 16; i++) {
		//	float r = 2;
		//	float theta = myMap(i, 0, 16, 0, PI);
		//	float x = r * cos(theta);
		//	float z = r * sin(theta);
		//	for (int j = 0; j < 16; j++) {
		//		float y = (j ) / 16.0f - 0.5f;
		//		Mat3 m = Mat3::RotationY(angley);
		//		Vec3 v(x,y,z);
		//		v = v * m;
		//		v.z += 1;
		//		sst.TransformSphere(v);
		//		gfx.drawPointDepth(v, (i * 255 / 16) % 128, j * 255 / 16, (i * 255 / 32) % 255);
		//	}
		//}


		solidCube();
		//wireFrame();
		//grid();

	}



	void grid() {
		for (int i = 0; i < 16; i++) {
			float x = (i * 2.0) / 16.0f - 1.0f;
			for (int j = 0; j < 8; j++) {
				float y = (j * 2.0) / 16.0f - 0.5f;
				Vec3 v(x *8, y *8, 0);

				Mat3 m = Mat3::RotationY(-anglex);
				v = v * m;
				float xOff = cameraOffset * cos(anglex+PI/2);
				float zOff = cameraOffset * sin(anglex+PI/2);
				v.x += xOff;
				v.z += zOff;
				//v.z += 2;
				sst.TransformSphere(v);
				gfx.drawPointDepth(v, (i * 255 / 16) % 128, j * 255 / 16, (i * 255 / 32) % 255);

			}
		}
	}

	void solidCube() {
		auto it = cube.getTriangles<Vertex>(2.0f);
		for (auto &v : it.vertices) {
			Mat3 m = Mat3::RotationX(anglex)  * Mat3::RotationY(angley);
			v.pos = v.pos * m;
			float xOff = cameraOffset * cos(anglex);
			float zOff = cameraOffset * sin(anglex);
			v.pos.x += xOff;
			v.pos.z += zOff;
		}
		pipeline.Draw(it);
	}

	void wireFrame() {
		auto lines = cube.getLines(2.0f);
		for (auto &v : lines.vertices) {
			Mat3 m = Mat3::RotationY(angley)  * Mat3::RotationX(anglex);
			v = v * m;
			//v.z -= cameraOffset;
			float xOff = cameraOffset * cos(anglex+PI);
			float zOff = cameraOffset * sin(anglex+PI);
			v.x += xOff;
			v.z += zOff;
			sst.TransformSphere(v);
		}
		for (size_t i = 0; i < lines.indices.size() / 2; i++) {
			gfx.drawLineDepth(lines.vertices[lines.indices.at(i * 2)], lines.vertices[lines.indices.at(i * 2 + 1)], 0, 50, 255);
		}
	}

	void drawTest() {
		gfx.resetZ();
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				Vec3 a(i, i, 0);
				Vec3 b(i + 30, i, 255);
				gfx.drawLineDepth(a, b, (i / 20.0) * 255, 255 - (i / 20.0) * 255, 50);
				//gfx->putPixel(i, i+j, (i / 20.0) * (j / 20.0) * 255);
			}
		}
	}

	void init() {
		gfx.init();
	}

	void clear() {
		gfx.clear();
	}

	void fade(unsigned char a) {
		gfx.fade(a);
	}

	void show() {
		gfx.show();
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

	Graphics &gfx;
	ScreenTransform sst;
	Pipeline<Vertex> pipeline;
};


