#pragma once

#include "Pattern.h"

//sample running http://neuroid.co.uk/lab/fluid/
//js http://neuroid.co.uk/lab/fluid/fluid.js
//processing https://github.com/weymouth/lily-pad
// forum talking about latice boltman calculations https://github.com/CodingTrain/Rainbow-Topics/issues/178

class PatternFluidSim : public _Pattern {
public:
	PatternFluidSim() : _Pattern("Fluid Sim") {}



	uint8_t drawFrame() {

		return returnVal;
	}

};