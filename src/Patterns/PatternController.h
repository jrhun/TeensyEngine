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
#include "PatternParticleSysOld.h"
#include "PatternPictures.h"
#include "PatternPurpleRain.h"
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
	PatternController() { gfx.setPalette(0); }

	//variables
	//uInt8Reference beatRef{ "Beat", _Pattern::beat, 0, 255, true };

	//add patterns as members here
	PatternRowThenCol	rowThenCol;
	PatternBeachWaves	beachWaves;
	PatternBeachWaves2	beachWaves2;
	PatternCube			cube3d;
	PatternDualWaves	dualWaves;
	PatternFire			fire;
	PatternFountain		particleFountain;

	PatternNoise1		noise1;
	PatternParametric	parametric;
	PatternParametric2	parametricSpiral;
	PatternPurpleRain	purpleRain;
	PatternRaymarcher	raymarcher;
	PatternSolid		solid;
	PatternSparks		sparks;
	PatternSpinningParticles	SpinningParticles;
	PatternSpiral		spiral;
	PatternSuperShape	superShape;
	PatternVWaves		verticalWaves;
	PatternWireFrame	wireFrame;
	PatternWheelBlur	wheelBlur;
	PatternWheelPart	wheelParticle;
	PatternWorld		world;
	


	// add pattern members to pattern list
	static const uint8_t numPatterns = 20;
    _Pattern *patternList[numPatterns] = {
		&dualWaves,
		&purpleRain,
		&wheelParticle,
		&wheelBlur,
		&SpinningParticles,
		
		&noise1,
		&verticalWaves,
		&beachWaves2,
		&beachWaves,
		&rowThenCol,
		&solid,
		&sparks,
		
		&parametricSpiral,
		&parametric,
		&spiral,

		&particleFountain,
		&fire, 
		&world,
		
		&wireFrame,
		&cube3d,
		
		//&raymarcher,
		//&superShape
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
	unsigned long nextCursorUpdate = 0;
	uint8_t cursorPos = SCREEN_WIDTH;

	void run() {
		if (nextUpdate == 0) {
			getCurrentPattern()->start();// initial
		}

		unsigned long now = GET_MILLIS();
		if (now > nextUpdate) {
			//        audio.readAudio();
			
			gfx.updatePalette();

			nextUpdate = getCurrentPattern()->drawFrame() + now; //runs on gfx led buffer

			//add effects
			if (_Pattern::useCustomEffect) {
				if (_Pattern::fadeFx)
					gfx.fade(_Pattern::fadeFx);
				if (_Pattern::blurFx)
					gfx.blur(_Pattern::blurFx);
				//if (FxSpiral)
				//	SpiralStream(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_HEIGHT, FxSpiral);
				//if (FxNoiseSmear)
				//	standardNoiseSmearing();
			}

			gfx.update(); ////copys from gfx led buffer to FastLED/screen buffer

			//text
			//gfx.setFont(&Org_01);
			
			//gfx.print("Meredith!");
			if (Data::textOn) {
				gfx.setCursor(cursorPos, 12);
				
				String text;
				if (Data::textIndex == Data::customIndex1) {
					text = Data::custom1;
				}
				else if (Data::textIndex == Data::customIndex2) {
					text = Data::custom2;
				}
				else {text = Data::textOptions[Data::textIndex];}
				gfx.print(text);
			}

			gfx.show();

		}
		if (Data::hueChange and now - nextHueUpdate > 1000 / 60) {
			nextHueUpdate = now;
			Data::incHue();
		}

		if (now - nextCursorUpdate > 1000 / 40) {
			nextCursorUpdate = now;
			cursorPos = (cursorPos - 1 + SCREEN_WIDTH) % SCREEN_WIDTH;
		}

	}
};



PatternController patterns;

VariableReference currentPattern_t{	"Current pattern", &Data::currentPattern, 0, 0, patterns.numPatterns };


