#pragma once

class PatternAudio1 : public _Pattern {
public:
	PatternAudio1() : _Pattern("Audio 1") {}

	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
				gfx.fade(70);
		}
	
	
		return returnVal;
	}
	
};