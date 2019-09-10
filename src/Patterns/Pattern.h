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
#else 
#include "FastLED_PC.h"
#include "../GraphicsPC.h"
GraphicsPC gfx;

#endif

#include "../Engine.h"
Engine engine(gfx);


#include "../Effects.h"
#include "../Particle.h"

//#include "../VariableControl.h"




// thoughts

// spinning circles, multiple

// particle system + perlin noise
// perline noise gives us "hot spots" that are more likely to generate particles
// either with a random direction or a direction according to perlin noise
// could give an interesting "scalar field" effect

// can also use perlin noise to generate smoke like effect

// replace uses of random with perlin noise, or perlin + random
// i.e higher perlin noise is added to random, to give some extra structure to noise





class Pattern {
public:
	Pattern() {}
	Pattern(const char* name) : name(name) {}

	virtual uint8_t drawFrame() {
		return 0;
	}

	virtual void start() {}
	virtual void stop() {}

	const char* getName() {
		return name;
	}

	const char* name;




	//uint8_t numVars = 0;
	//uint8_t getNumVars() {
	//	return numVars;
	//}

	//char* getVarName(uint8_t i) {
	//	if (i >= 0 and i < getNumVars()) {
	//		return 
	//	}
	//}

	static bool useDefaultEffect;

};

bool Pattern::useDefaultEffect = true;
