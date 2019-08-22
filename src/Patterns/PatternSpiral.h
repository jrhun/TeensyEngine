#pragma once

#include "Pattern.h"

#if defined(ARDUINO)
#else
//#include "FastLED_PC.h"

#endif


class PatternSpiral : public Pattern {
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
	PatternSpiral() {
		name = (char *)"Spiral";
	}

	void start() {
		// set all counting directions positive for the beginning
		for (int i = 0; i < timers; i++) multiTimer[i].delta = 1;

		// set range (up/down), speed (takt=ms between steps) and starting point of all oszillators

		unsigned long now = GET_MILLIS();

		multiTimer[0].lastMillis = now;
		multiTimer[0].takt = 42;     //x1
		multiTimer[0].up = SCREEN_WIDTH - 1;
		multiTimer[0].down = 0;
		multiTimer[0].count = 0;

		multiTimer[1].lastMillis = now;
		multiTimer[1].takt = 55;     //y1
		multiTimer[1].up = SCREEN_HEIGHT - 1;
		multiTimer[1].down = 0;
		multiTimer[1].count = 0;

		multiTimer[2].lastMillis = now;
		multiTimer[2].takt = 3;      //color
		multiTimer[2].up = 255;
		multiTimer[2].down = 0;
		multiTimer[2].count = 0;

		multiTimer[3].lastMillis = now;
		multiTimer[3].takt = 71;     //x2  
		multiTimer[3].up = SCREEN_WIDTH - 1;
		multiTimer[3].down = 0;
		multiTimer[3].count = 0;

		multiTimer[4].lastMillis = now;
		multiTimer[4].takt = 89;     //y2
		multiTimer[4].up = SCREEN_HEIGHT - 1;
		multiTimer[4].down = 0;
		multiTimer[4].count = 0;
	}

	uint8_t drawFrame() {
		// manage the Oszillators
		UpdateTimers();

		gfx.fade(64);

		// draw just a line defined by 5 oszillators
		//Vec3 a(multiTimer[3].count, multiTimer[4].count, 255);
		//Vec3 b(multiTimer[0].count, multiTimer[1].count, 255);
		CRGB col = CHSV(multiTimer[2].count, 255, 255);
		gfx.drawLine(
			multiTimer[3].count,
			multiTimer[4].count,
			multiTimer[0].count,
			multiTimer[1].count,
			col);

		// manipulate the screen buffer
		// with fixed parameters (could be oszillators too)
		// center x, y, radius, scale color down
		// --> affects always a square with an odd length
		SpiralStream(15, 15, 16, 128);

		// why not several times?!
		SpiralStream(16, 6, 6, 128);
		SpiralStream(10, 24, 10, 128);

		// increase the contrast
		//gfx.fade(250);

		return 0;
	}
};