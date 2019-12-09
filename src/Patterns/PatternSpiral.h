#pragma once

#include "Pattern.h"

#if defined(ARDUINO)
#else
//#include "FastLED_PC.h"

#endif


class PatternSpiral : public _Pattern {
private:
	// Timer stuff (Oszillators)
	struct timer {
		unsigned long takt;
		unsigned long lastMillis;
		unsigned long count;
		int delta;
		byte up;
		byte down;
	};
	timer multiTimer[5];

	int timers = sizeof(multiTimer) / sizeof(multiTimer[0]);

	// counts all variables with different speeds linear up and down
	void UpdateTimers()
	{
		unsigned long now = GET_MILLIS();
		for (int i = 0; i < timers; i++)
		{
			while (now - multiTimer[i].lastMillis >= multiTimer[i].takt)
			{
				multiTimer[i].lastMillis += multiTimer[i].takt;
				multiTimer[i].count = multiTimer[i].count + multiTimer[i].delta;
				if ((multiTimer[i].count == multiTimer[i].up) || (multiTimer[i].count == multiTimer[i].down))
				{
					multiTimer[i].delta = -multiTimer[i].delta;
				}
			}
		}
	}

public:
	PatternSpiral() : _Pattern("Spiral") {}

	void start() {
		// set all counting directions positive for the beginning
		for (int i = 0; i < timers; i++) multiTimer[i].delta = 1;

		// set range (up/down), speed (takt=ms between steps) and starting point of all oszillators

		unsigned long now = GET_MILLIS();

		multiTimer[0].lastMillis = now;
		multiTimer[0].takt = 21;     //x1
		multiTimer[0].up = SCREEN_WIDTH - 1;
		multiTimer[0].down = 0;
		multiTimer[0].count = 0;

		multiTimer[1].lastMillis = now;
		multiTimer[1].takt = 44;     //y1
		multiTimer[1].up = SCREEN_HEIGHT - 1;
		multiTimer[1].down = 0;
		multiTimer[1].count = 0;

		multiTimer[2].lastMillis = now;
		multiTimer[2].takt = 20;      //color
		multiTimer[2].up = 255;
		multiTimer[2].down = 0;
		multiTimer[2].count = 0;

		multiTimer[3].lastMillis = now;
		multiTimer[3].takt = 51;     //x2  
		multiTimer[3].up = SCREEN_WIDTH - 1;
		multiTimer[3].down = 0;
		multiTimer[3].count = 0;

		multiTimer[4].lastMillis = now;
		multiTimer[4].takt = 79;     //y2
		multiTimer[4].up = SCREEN_HEIGHT - 1;
		multiTimer[4].down = 0;
		multiTimer[4].count = 0;
	}

	uint8_t drawFrame() {
		// manage the Oszillators
		UpdateTimers();

		

		//shift right 
		//CRGB column[SCREEN_HEIGHT];
		//for (int i = 0; i < SCREEN_HEIGHT; i++) {
		//	column[i] = leds[SCREEN_WIDTH - 1 + i * SCREEN_WIDTH];// save right most leds
		//}
		//for (int i = SCREEN_WIDTH; i > 0; i--) {
		//	for (int j = 1; j < SCREEN_HEIGHT; j++) {
		//		leds[i + j * SCREEN_WIDTH] = leds[i - 1 + j * SCREEN_WIDTH];
		//	}
		//}
		//for (int i = 0; i < SCREEN_HEIGHT; i++) {
		//	leds[0 + i * SCREEN_WIDTH] = column[i];
		//}

		// draw just a line defined by 5 oszillators
		//CRGB col = CHSV(multiTimer[2].count, 255, 255);
		gfx.drawLine(
			multiTimer[3].count,
			multiTimer[4].count,
			multiTimer[0].count,
			multiTimer[1].count,
			multiTimer[2].count,
			multiTimer[2].count+64);

		float length = beatsin8(5, 10, 20);
		if (random8(0, 150) == 0) {
			dir = -dir;
		}
		angle += dir * (beatsin8(20, 2, 3) / 30.0);
		a.x = length * cos(angle) / 2 + SCREEN_WIDTH / 2 + xOffset;
		a.y = length * sin(angle) / 2 + SCREEN_HEIGHT / 2;
		b.x = -length * cos(angle) / 2 + SCREEN_WIDTH / 2 + xOffset;
		b.y = -length * sin(angle) / 2 + SCREEN_HEIGHT / 2;

		Vec3 offset(multiTimer[3].count, multiTimer[4].count, 0);
		//engine.sst.Transform(a);
		//engine.sst.Transform(b);
		//gfx.drawLine(a.x, a.y+beatsin8(10,0,10)-5, b.x, b.y + beatsin8(10, 0, 10)-5, 0, length * 4);


		//gfx.drawLineDepth(a + offset, b + offset, 0, length * 4);
		// manipulate the screen buffer
		// with fixed parameters (could be oszillators too)
		// center x, y, radius, scale color down
		// --> affects always a square with an odd length

		
		if (!_Pattern::useCustomEffect) {
			SpiralStream(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_HEIGHT, 128);
			// increase the contrast
			gfx.fade(5);
		}
		//SpiralStream(3*SCREEN_WIDTH/4, SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, GuiVars1 * 127);

		// why not several times?!
		SpiralStream(16, 6, 6, GuiVars2 * 127);
		SpiralStream(10, 24, 10, GuiVars3 * 127);


		xOffset += beatsin8(30, 1 , 10) / 10.0;
		//angle += 0.1;
		if (angle > TWO_PI)
			angle -= TWO_PI;
		if (angle < 0)
			angle += TWO_PI;
		if (xOffset > SCREEN_WIDTH)
			xOffset -= SCREEN_WIDTH;

		return returnVal;
	}

	Vec3 a = Vec3(0, 0, 255);
	Vec3 b = Vec3(0, 0, 255);
	float angle = 0;
	float xOffset = 0;
	int dir = 1;
};
