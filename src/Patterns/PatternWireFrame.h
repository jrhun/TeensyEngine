#pragma once

#include "Pattern.h"

class PatternWireFrame : public _Pattern {
public: 
	PatternWireFrame() : _Pattern("Wireframe") {}

	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
			int beat = beatsin8(2, 80, 224);
			int beat2 = beatsin8(2, 224, 80);
			gfx.fade(beat);
			gfx.blur(beat2);
		}
		gfx.resetZ();
		auto lines1 = cube.getLines(2.0f);
		auto lines2 = cube.getLines(2.0f);
		Mat3 m1 = Mat3::RotationY(angle)  * Mat3::RotationX(angle);
		float xOff = (cameraOffset + 3) * cos(angle + PI);
		float zOff = (cameraOffset + 3) * sin(angle + PI);
		for (auto &v : lines1.vertices) {
			v = v * m1;
			v.x += xOff;
			v.z += zOff;
			engine.sst.TransformSphere(v);
		}
		Mat3 m2 = Mat3::RotationX(angle)  * Mat3::RotationY(angle);
		xOff = (-cameraOffset + 3) * cos(angle);
		zOff = (-cameraOffset + 3) * sin(angle);
		for (auto &v : lines2.vertices) {
			v = v * m2;			
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

		return returnVal;
	}

	float cameraOffset = 0;
	float offsetDir = 1.0;
	float angle = 0;
	Cube cube;
};
