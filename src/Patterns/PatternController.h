#pragma once

// data stores variables that are changed by UI or automatically update (e.g. base hue)
#include "../../data.h"

// include all patterns
#include "Pattern.h"
#include "PatternBeachWaves.h"
#include "PatternSpiral.h"
#include "PatternCube.h"



class PatternController {
public:
	PatternController() {}

	//add patterns as members here
	PatternBeachWaves	beachWaves;
	PatternSpiral		spiral;
	PatternCube			cube3d;

	// add pattern members to pattern list
	static const uint8_t numPatterns = 3;
	Pattern *patternList[numPatterns] = {
		&spiral,
		&beachWaves,
		&cube3d
		
	};

	char* getCurrentPatternName() {
		return getCurrentPattern()->getName(); //patternList[Data::currentPattern].getName();
	}
	char* getPatternName(uint8_t i) {
		if (i < size()) return  patternList[i]->getName();
		else return (char*)"Invalid";
	}
	uint8_t size() {
		return numPatterns;//return ARRAY_SIZE(patternList);
	}
	void set(uint8_t i) {
		getCurrentPattern()->stop();
		if (i < size()) Data::currentPattern = i;
		getCurrentPattern()->start();
	}
	void inc() {
		getCurrentPattern()->stop();
		Data::currentPattern = (Data::currentPattern + 1) % size();
		getCurrentPattern()->start();
	}
	void dec() {
		getCurrentPattern()->stop();
		if (Data::currentPattern - 1 < 0) Data::currentPattern = size() - 1;
		else Data::currentPattern--;
		getCurrentPattern()->start();
	}

	Pattern* getCurrentPattern() {
		return patternList[Data::currentPattern];
	}

	unsigned long nextUpdate = 0;
	unsigned long nextHueUpdate = 0;

	void run() {
		if (nextUpdate == 0) {
			getCurrentPattern()->start();// initial
		}
		unsigned long now = GET_MILLIS();
		if (now > nextUpdate) {
			//        audio.readAudio();

			nextUpdate = getCurrentPattern()->drawFrame() + now;


			//        memcpy(&scratchArray[0], &leds[0], NUM_LEDS * sizeof(leds[0]));
			//        leds.displayText();
			//        memcpy(&leds[0], &scratchArray[0], NUM_LEDS * sizeof(leds[0]));
#if defined(ARDUINO)
			FastLED.setBrightness(Data::brightness);
			//FastLED.show();
#else 
			//gfx.show();
#endif
			gfx.show();
		}
		if (now - nextHueUpdate > 1000 / 60) {
			nextHueUpdate = now;
			Data::incHue();
		}

	}
};

PatternController patterns;