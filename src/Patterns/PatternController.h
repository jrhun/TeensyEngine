#pragma once

// data stores variables that are changed by UI or automatically update (e.g. base hue)
#include "../../data.h"

// include all patterns
#include "Pattern.h"
#include "PatternAudio.h"
#include "PatternBasic.h"
#include "PatternBeachWaves.h"
#include "PatternCube.h"
#include "PatternFire.h"
#include "PatternFireworks.h"
#include "PatternFluidSim.h"
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
	PatternAudio1		audio1;
	PatternRowThenCol	rowThenCol;
	PatternBlank		blank;
	PatternBeachWaves	beachWaves; //fine 
	PatternBeachWaves2	beachWaves2; //issue - seems to be with blur2d
	PatternCube			cube3d;
	PatternDualWaves	dualWaves;
	PatternFish			fish;
	PatternFire			fire;
	PatternFireworks	fireworks;
	PatternFountain		particleFountain;

	PatternNoise1		noise1;
	PatternParametric	parametric; //issue
	PatternParametric2	parametricSpiral; //issue
	ParticleSystemWater	particlesWater;
	PatternPurpleRain	purpleRain;
	PatternRain			rain;
	PatternRaymarcher	raymarcher;
	PatternSolid		solid;
	PatternSparks		sparks;
	PatternSpinningParticles	SpinningParticles;
	PatternSpiral		spiral;
	PatternSuperShape	superShape;
	PatternVWaves		verticalWaves;
	PatternWireFrame	wireFrame;
	PatternWheelBlur	wheelBlur; //issue
	PatternWheelPart	wheelParticle; //issue
	PatternWorld		world;
	PatternWorld2		worldNoWater;
	PatternMario		mario; 
	PatternEyes			eyes; 
	PatternFlowers		flowers;
	PatternTest			test;

	PatternFluidSim		fluidSim; 
	


	// add pattern members to pattern list
	static const uint8_t numPatterns = 31;
    _Pattern *patternList[numPatterns] = {	
//		&test,
		&fluidSim,

		&blank,
		&audio1,
		&fireworks,
		&fish,
		&particlesWater,
		&dualWaves, //issue
		&purpleRain,
		&rain, 
		&wheelParticle,	//issue
		&wheelBlur, //issue
		&SpinningParticles,
		
		&noise1,
		&verticalWaves,
		&beachWaves2,//issue
		&beachWaves, //issue
		&rowThenCol,
		&solid,
		&sparks,
		
		&parametricSpiral,
		&parametric,
		&spiral, //issue

		&particleFountain,
		&fire, 
		&world,
		&worldNoWater,
		&mario,
		&eyes,
		&flowers,
		
		&wireFrame, //issue intermittently 
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
	unsigned long nextPaletteUpdate = 0;
	uint8_t cursorPos = SCREEN_WIDTH;

	uint8_t blinkTimer = 0;


	void blinkText() {
		blinkTimer = 64;
	}

	void run() {
		if (nextUpdate == 0) {
			getCurrentPattern()->start();// initial
		}

		unsigned long now = GET_MILLIS();
		if (now > nextUpdate) {
			//        audio.readAudio();
			
			gfx.updatePalette();


			//add effects to previous frame
			if (_Pattern::useCustomEffect) {
				if (_Pattern::fadeFx)
					gfx.fade(_Pattern::fadeFx);
				if (_Pattern::blurFx)
					gfx.blur(_Pattern::blurFx);
				if (_Pattern::spiralFx)
					gfx.spiral(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_HEIGHT, _Pattern::spiralFx);
				//	SpiralStream(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_HEIGHT, FxSpiral);
				//if (FxNoiseSmear)
				//	standardNoiseSmearing();
			}

			nextUpdate = getCurrentPattern()->drawFrame() + now; //runs on gfx led buffer

			

			gfx.update(); ////copys from gfx led buffer to FastLED/screen buffer

			//text
			if (Data::textOn or blinkTimer) {
				uint8_t oldOpacity = gfx.textOpacity;

				if (blinkTimer) {
					gfx.textOpacity = decayData[blinkTimer];
					blinkTimer--;
				}

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

				gfx.textOpacity = oldOpacity;
			}
			
			if (_Pattern::useCustomEffect and _Pattern::glitterFx) {
			//can put in sampleavg here
				const uint8_t num = 40;
				static uint16_t glitter[num];
				static uint8_t glitterVal[num];
				static uint8_t lastGlitterPos = 0;
				if (random8() < _Pattern::glitterFx * 10) {
				uint8_t x,y;
				x = random8(SCREEN_WIDTH);
				y = random8(SCREEN_HEIGHT);
					gfx.putPixelDirect(x, y, CRGB::White);
					glitter[lastGlitterPos] = x << 8 | y;
					glitterVal[lastGlitterPos] = 63;
					lastGlitterPos = (lastGlitterPos + 1) % num;
				}
				uint8_t count = 0;
				for (uint8_t i = 0; i < num; i++) {
					if (glitterVal[i]) {
						count++;
						glitterVal[i]--;
						if (glitterVal[i] < 32) {
							glitterVal[i] = 0;
							continue;
						}
						CRGB c;
						
						uint8_t v = gamma6[glitterVal[i]];
						c.r = v;
						c.g = v;
						c.b = v;
						gfx.putPixelDirect(uint8_t(glitter[i] >> 8), uint8_t(glitter[i]), c);
					}
				}
			}

			gfx.show();

		}
		if (Data::hueChange and (now - nextHueUpdate > (1000 / Data::hueSpeed)) ) {
			nextHueUpdate = now;
			Data::incHue();
		}

		if (now - nextCursorUpdate > 1000 / gfx.textSpeed) {
			nextCursorUpdate = now;
			cursorPos = (cursorPos - 1 + SCREEN_WIDTH) % SCREEN_WIDTH;
		}
		
		if (Data::changePaletteDelay and (now - nextPaletteUpdate > (1000 * Data::changePaletteDelay))) {
			nextPaletteUpdate = now;
			gfx.setPalette(random8(gGradientPaletteCount));
		}

	}
};



PatternController patterns;

VariableReference currentPattern_t{	"Current pattern", &Data::currentPattern, 0, 0, patterns.numPatterns };


