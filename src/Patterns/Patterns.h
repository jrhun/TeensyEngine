#pragma once

//#if defined(ARDUINO)
//typedef String String;
//#else
//#include <string>
//typedef String std::string;
//#endif

#if defined (ARDUINO)
#include "FastLED.h"
#include "../../ledControl.h"

#else 
#include "FastLED_PC.h"
#endif




class Pattern {
public:
	Pattern() {}
	Pattern(char* name) : name(name) {}

	virtual uint8_t drawFrame() = 0;
	char* getName() {
		return name;
	}

	char* name;
};
