#pragma once

#include "Pattern.h"



class PatternAudio1 : public _Pattern {
public:
	PatternAudio1() : _Pattern("VU Meter") {}

	uint8_t drawFrame() {

		if (!_Pattern::useCustomEffect) {
			gfx.fade(140);
		}

		uint16_t tempsamp = constrain(Data::sampleAvg, 0, Data::audioThreshold*2);
		tempsamp = myMap(tempsamp, 0, Data::audioThreshold*2, 4, SCREEN_HEIGHT-1, true); // 0-31

		uint8_t peak = myMap(Data::sampleArray[(Data::sampleCount+ Data::numSamples-1) % Data::numSamples], 0, Data::audioThreshold * 2, 0, SCREEN_HEIGHT - 1, true);

		v[x] = max(constrain(peak, 0, 16), random8(1,3));
		v[(x + SCREEN_WIDTH/2) % SCREEN_WIDTH] = max(constrain(peak, 0, 16), random8(1, 3));;

		for (int j = 0; j < SCREEN_WIDTH; j++) {
			for (int i = 0; i < v[j]; i++) { //v[j] is from 0 to 31
				gfx.putPixel(j, SCREEN_HEIGHT - i - 1, gfx.getColour(j * 2 + i/2));

			//for (int i = 0; i < tempsamp; i++) {
				//uint8_t index = inoise16(i * Data::sampleAvg + GET_MILLIS(), 5000 + i * Data::sampleAvg)>>8;
				//gfx.putPixel(j, SCREEN_HEIGHT - i - 1, gfx.getColour(index + j*2 + x));
			}
		}

		for (int i = 0; i < SCREEN_WIDTH; i++) {
			if (v[i] > 4 and random8(1)) {
				v[i]--;
			}
			if (random8(12) == 0) {
				if (v[i] < SCREEN_HEIGHT/4) 
					v[i] += random8(3);
			}
		}


		x++;
		x %= SCREEN_WIDTH;

		//if (tempsamp >= topLED)
		//	topLED = tempsamp;
		////else if (gravityCounter % GRAVITY == 0 and topLED > 0)
		//else if (gravityCounter > 1.0f) {
		//	if (topLED > 0)
		//		topLED--;
		//	gravityCounter = 0.0f;
		//}
		//else {
		//	gravityCounter += 0.8f;
		//}
		//if (topLED > 0) {
		//	for (int i = 0; i < SCREEN_WIDTH; i++)
		//		gfx.putPixel(i, SCREEN_HEIGHT - 1 - topLED, CHSV(Data::getHue(), 255, 255));
		//}




		//gravityCounter = (gravityCounter + 1) % GRAVITY;


		return returnVal;
	}

	uint8_t topLED = 0;
	uint8_t x;
	uint8_t v[SCREEN_WIDTH]; 
	float gravityCounter = 0.0;
	const uint8_t GRAVITY = 2;
	
};


class PatternAudio2 : public _Pattern {
public:
	PatternAudio2() : _Pattern("VU Meter") {}

	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
			gfx.fade(140);
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