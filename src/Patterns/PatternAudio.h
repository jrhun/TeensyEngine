#pragma once

#include "Pattern.h"



class PatternAudio1 : public _Pattern {
public:
	PatternAudio1() : _Pattern("VU Meter") {}

	uint8_t drawFrame() {

		if (!_Pattern::useCustomEffect) {
			gfx.fade(160);
		}

		uint16_t tempsamp = constrain(Data::sampleAvg, 0, Data::audioThreshold);
		tempsamp = myMap(tempsamp, 0, Data::audioThreshold, 0, SCREEN_WIDTH);


		for (int j = 0; j < SCREEN_WIDTH; j++) {
			for (int i = 0; i < tempsamp; i++) {
				uint8_t index = inoise8(i*Data::sampleAvg + GET_MILLIS(), 5000 + i * Data::sampleAvg);
				gfx.putPixel(j, SCREEN_HEIGHT - i - 1, CHSV(index, 255, constrain(Data::sampleAvg, 64, 255)));
			}
		}

		if (tempsamp >= topLED)
			topLED = tempsamp;
		else if (gravityCounter % GRAVITY == 0 and topLED > 0)
			topLED--;

		if (topLED > 0) {
			for (int i = 0; i < SCREEN_WIDTH; i++)
				gfx.putPixel(i, SCREEN_HEIGHT - 1 - topLED, CHSV(Data::getHue(), 255, 255));
		}

		gravityCounter = (gravityCounter + 1) % GRAVITY;


		return returnVal;
	}

	uint8_t topLED = 0;
	int gravityCounter = 0;
	const uint8_t GRAVITY = 2;
	
};


class PatternAudio2 : public _Pattern {
public:
	PatternAudio2() : _Pattern("VU Meter") {}

	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
			gfx.fade(160);
		}

		uint16_t tempsamp = constrain(Data::sampleAvg, 0, Data::audioThreshold);
		tempsamp = myMap(tempsamp, 0, Data::audioThreshold, 0, SCREEN_WIDTH);
		

		for (int j = 0; j < SCREEN_WIDTH; j++) {
			for (int i = 0; i < tempsamp; i++) {
				uint8_t index = inoise8(i*Data::sampleAvg + GET_MILLIS(), 5000 + i * Data::sampleAvg);
				gfx.putPixel(j, SCREEN_HEIGHT - i - 1, CHSV(Data::getHue(), 255, Data::sampleAvg));
			}
		}

		if (tempsamp >= topLED)
			topLED = tempsamp;
		else if (gravityCounter % GRAVITY == 0 and topLED > 0)
			topLED--;

		if (topLED > 0) {
			for (int i = 0; i < SCREEN_WIDTH; i++)
				gfx.putPixel(i, SCREEN_WIDTH - 1 - topLED, CHSV(Data::getHue(), 255, 255));
		}

		gravityCounter = (gravityCounter + 1) % GRAVITY;


		return returnVal;
	}

	uint8_t topLED;
	int gravityCounter = 0;
	const uint8_t GRAVITY = 2;

};