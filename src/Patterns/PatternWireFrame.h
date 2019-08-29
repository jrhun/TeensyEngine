#pragma once

#include "Pattern.h"

class PatternWireFrame : public Pattern {
public: 
	PatternWireFrame() : Pattern("Wireframe") {}

	uint8_t drawFrame() {
		//gfx.fade(beatsin8(2, 48, 224));
		blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, GuiVars1 * 127.0);
		gfx.resetZ();
		auto lines1 = cube.getLines(2.0f);
		auto lines2 = cube.getLines(2.0f);
		for (auto &v : lines1.vertices) {
			Mat3 m = Mat3::RotationY(angle)  * Mat3::RotationX(angle);
			v = v * m;
			//v.z -= cameraOffset;
			float xOff = (cameraOffset+3) * cos(angle + PI);
			float zOff = (cameraOffset+3) * sin(angle + PI);
			v.x += xOff;
			v.z += zOff;
			engine.sst.TransformSphere(v);
		}
		for (auto &v : lines2.vertices) {
			Mat3 m = Mat3::RotationX(angle)  * Mat3::RotationY(angle);
			v = v * m;
			//v.z -= cameraOffset;
			float xOff = (-cameraOffset + 3) * cos(angle);
			float zOff = (-cameraOffset + 3) * sin(angle);
			v.x += xOff;
			v.z += zOff;
			engine.sst.TransformSphere(v);
		}
		auto getIndicesHue = [](int i) {
			switch (i) {
				//front face
			case 0:
				return 0;
			case 1:
			case 2:
			case 4:
				return 24;
				//back face
			case 3:
			case 5:
			case 6:
				return 48;
			case 7:
				return 72;
			default:
				return 0;
			}
		};
		for (size_t i = 0; i < lines1.indices.size() / 2; i++) {
			uint8_t hue1 = getIndicesHue(lines1.indices[(i * 2)]);
			uint8_t hue2 = getIndicesHue(lines1.indices[(i * 2 + 1)]);
			gfx.drawLineDepth(lines1.vertices[lines1.indices[(i * 2)]], lines1.vertices[lines1.indices[(i * 2 + 1)]], hue1, hue2);
		}
		for (size_t i = 0; i < lines2.indices.size() / 2; i++) {
			uint8_t hue1 = getIndicesHue(lines2.indices[(i * 2)]) + 64;
			uint8_t hue2 = getIndicesHue(lines2.indices[(i * 2 + 1)]) + 64;
			gfx.drawLineDepth(lines2.vertices[lines2.indices[(i * 2)]], lines2.vertices[lines2.indices[(i * 2 + 1)]], hue1, hue2);
		}

		angle += 0.01;
		if (angle >= TWO_PI)
			angle -= TWO_PI;

		if (cameraOffset > 1)
			offsetDir = -1.0f;
		if (cameraOffset < -1)
			offsetDir = 1.0f;
		cameraOffset += 0.01 * offsetDir;

		return 0;
	}

	float cameraOffset = 0;
	float offsetDir = 1.0;
	float angle = 0;
	Cube cube;
};