#pragma once

//#if defined(ARDUINO)
//typedef String String;
//#else
//#include <string>
//typedef String std::string;
//#endif

#if defined(ESP32) || defined(CORE_TEENSY)
#include "FastLED.h"
#include "../../ledControl.h"
#include "../GraphicsFastLED.h"
GraphicsFastLED gfx(&ledControl);
#define returnVal 10
#else 
#include "FastLED_PC.h"
#include "../GraphicsPC.h"
GraphicsPC gfx;
#define returnVal 0
#endif

#include "../Engine.h"
Engine engine(gfx);


#include "../Palettes.h"
#include "../Effects.h"
#include "../Particle.h"
#include "../Noise.h"

#include "../VariableControl.h"




// thoughts

// spinning circles, multiple

// particle system + perlin noise
// perline noise gives us "hot spots" that are more likely to generate particles
// either with a random direction or a direction according to perlin noise
// could give an interesting "scalar field" effect

// can also use perlin noise to generate smoke like effect

// replace uses of random with perlin noise, or perlin + random
// i.e higher perlin noise is added to random, to give some extra structure to noise





class _Pattern {
public:
	_Pattern() {}
	_Pattern(const char* name) : name(name) {
		Data::triggerType_t.setCallback([] {
			beat.setType(*Data::triggerType_t);
		});
	}

	virtual uint8_t drawFrame() {
		updateVars();
		return 0;
	}

	virtual void start() {}
	virtual void stop() {}

	const char* getName() {
		return name;
	}

	const char* name;



	static const uint8_t maxVars = 6;
	
	uint8_t numVars = 0;
	uint8_t getNumVars() {
		return numVars;
	}

	VariableReference *vars[maxVars];

	void updateVars() {
		//for (uint8_t i = 0; i < getNumVars(); i++) {
		//	vars[i]->update();
		//}
		beat.update();
	}

	void trigger() {
		beat.sync();
	}


	VariableReference *getVar(uint8_t i) {
		if (i >= 0 and i < getNumVars()) {
			return vars[i];
		}
		return nullptr;
	}

	String getVarName(uint8_t i) {
		if (i >= 0 and i < getNumVars()) {
			return vars[i]->getName() + ": ";
		} 
		return "";
	}

	String getVarValue(uint8_t i) {
		if (i >= 0 and i < getNumVars()) {
			return vars[i]->getValue();
		}
		return "";
	}

	static bool useDefaultEffect;

	static VariableOscilate beat;

};

VariableOscilate _Pattern::beat = VariableOscilate();
bool _Pattern::useDefaultEffect = true;
