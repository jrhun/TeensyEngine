#pragma once

#include "Pattern.h"
#include "../Cube.h"
#include <math.h>


class PatternCube : public Pattern {
public: 
	PatternCube() : Pattern("Cube 3d")  {
	}

	uint8_t drawFrame() {

		if (Pattern::useDefaultEffect) {
			gfx.fade(128);
		}
		//blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, GuiVars1 * 127.0);
		engine.resetZ();
		auto it1 = cube.getTriangles<Vertex>(2.0f);
		auto it2 = cube.getTriangles<Vertex>(2.0f);
		for (auto &v : it1.vertices) {
			Mat3 m = Mat3::RotationX(angle)  * Mat3::RotationY(angle);
			v.pos = v.pos * m;
			v.pos.x += (cameraOffset + 3) * cos(angle + PI);
			v.pos.z += (cameraOffset + 3) * sin(angle + PI);
		}

		for (auto &v : it2.vertices) {
			Mat3 m = Mat3::RotationY(angle)  * Mat3::RotationX(angle);
			v.pos = v.pos * m;
			v.pos.x += (-cameraOffset+3) * cos(angle);
			v.pos.z += (-cameraOffset+3) * sin(angle);
		}

		engine.pipeline.Draw(it1);
		engine.pipeline.Draw(it2);

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

	float cameraOffset = 0.0;
	float offsetDir = 1.0;
	float angle = 0;
	Cube cube;
	//VariableControl<float> v; 
};