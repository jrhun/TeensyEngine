#pragma once

#include "Pattern.h"

class PatternWireFrame : public Pattern {
public: 
	PatternWireFrame() : Pattern("Wireframe") {}

	uint8_t drawFrame() {
		gfx.fade(beatsin8(2, 48, 224));
		gfx.resetZ();
		auto lines = cube.getLines(2.0f);
		for (auto &v : lines.vertices) {
			Mat3 m = Mat3::RotationY(angle)  * Mat3::RotationX(angle);
			v = v * m;
			//v.z -= cameraOffset;
			float xOff = cameraOffset * cos(angle + PI);
			float zOff = cameraOffset * sin(angle + PI);
			v.x += xOff;
			v.z += zOff;
			engine.sst.TransformSphere(v);
		}
		for (size_t i = 0; i < lines.indices.size() / 2; i++) {
			gfx.drawLineDepth(lines.vertices[lines.indices.at(i * 2)], lines.vertices[lines.indices.at(i * 2 + 1)], 0, 50, 255);
		}

		angle += 0.01;
		if (angle >= TWO_PI)
			angle -= TWO_PI;

		if (cameraOffset > 4)
			offsetDir = -1.0f;
		if (cameraOffset < 2)
			offsetDir = 1.0f;
		cameraOffset += 0.01 * offsetDir;

		return 0;
	}

	float cameraOffset;
	float offsetDir = 1.0;
	float angle = 0;
	Cube cube;
};