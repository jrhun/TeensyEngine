#pragma once

// data stores variables that are changed by UI or automatically update (e.g. base hue)
#include "../../data.h"

// include all patterns
#include "Pattern.h"
#include "PatternBasic.h"
#include "PatternBeachWaves.h"
#include "PatternCube.h"
#include "PatternFire.h"
#include "PatternNoise.h"
#include "PatternParametric.h"
#include "PatternPictures.h"
#include "PatternRaymarcher.h"
#include "PatternSpiral.h"
#include "PatternSuperShape.h"
#include "PatternWireFrame.h"
#include "PatternWheel.h"

//todo next
//BPM generator/trigger
// types
// -exponential decay
// -sin
// -ramp
// -triangle
// link to a variable in each pattern

// patterns
// DNA spiral
// pictures



class PatternController {
public:
	PatternController() {}

	//variables
	uInt8Reference beatRef{ "Beat", _Pattern::beat, 0, 255, true };

	//add patterns as members here
	PatternRowThenCol	rowThenCol;
	PatternBeachWaves	beachWaves;
	PatternBeachWaves2	beachWaves2;
	PatternCube			cube3d;
	PatternFire			fire;
	PatternNoise1		noise1;
	PatternParametric	parametric;
	PatternParametric2	parametricSpiral;
	PatternRaymarcher	raymarcher;
	PatternSparks		sparks;
	PatternSpiral		spiral;
	PatternSuperShape	superShape;
	PatternVWaves		verticalWaves;
	PatternWireFrame	wireFrame;
	PatternWheelBlur	wheelBlur;
	PatternWheelPart	wheelParticles;
	PatternWorld		world;


	// add pattern members to pattern list
	static const uint8_t numPatterns = 16;
    _Pattern *patternList[numPatterns] = {
		&verticalWaves,
		&noise1,
		&parametricSpiral,
		
		&sparks,
		//&superShape,
		&rowThenCol,
		&raymarcher,
		&world,
		
		&wheelParticles,
		&fire, 
		&beachWaves2,
		
		&wheelBlur,
		&parametric,
		&spiral,
		&wireFrame,
		&cube3d,
		&beachWaves
		
		
	};

	const char* getCurrentPatternName() {
		return getCurrentPattern()->getName(); //patternList[Data::currentPattern].getName();
	}
	const char* getPatternName(uint8_t i) {
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

	_Pattern* getCurrentPattern() {
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
			if (!_Pattern::useDefaultEffect) {
				if (FxFade)
					gfx.fade(FxFade);
				if (FxBlur)
					blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, FxBlur);
				if (FxSpiral)
					SpiralStream(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_HEIGHT, FxSpiral);
				if (FxNoiseSmear)
					standardNoiseSmearing();
			}

		}
		if (now - nextHueUpdate > 1000 / 60) {
			nextHueUpdate = now;
			Data::incHue();
		}

	}
};

PatternController patterns;

