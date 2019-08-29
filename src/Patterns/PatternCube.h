#pragma once

#include "Pattern.h"
#include "../Cube.h"
#include <math.h>


class PatternCube : public Pattern {
public: 
	PatternCube() : Pattern("Cube 3d")  {
	}

	uint8_t drawFrame() {

		if (Pattern::useDefaultEffect)
			gfx.fade(150);
		//blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, GuiVars1 * 127.0);
		engine.resetZ();
		auto it = cube.getTriangles<Vertex>(2.0f);
		for (auto &v : it.vertices) {
			Mat3 m = Mat3::RotationX(angle)  * Mat3::RotationY(angle);
			v.pos = v.pos * m;
			v.pos.x += cameraOffset * cos(angle);
			v.pos.z += cameraOffset * sin(angle);
		}
		engine.pipeline.Draw(it);

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

	float cameraOffset = 2.0;
	float offsetDir = 1.0;
	float angle = 0;
	Cube cube;
	//VariableControl<float> v; 
};