#pragma once




//#include <vector>
#include "../data.h"
#include "Patterns/PatternController.h"

#if defined(ESP32) || defined(CORE_TEENSY)
#include "FastLED.h"

#else 
#include "FastLED_PC.h"

#endif
// menu

#include "VariableControl.h"

#define MAX_LINES	8


String intToString(uint8_t i) {
#if defined(ESP32) || defined(CORE_TEENSY)
	return String(i);// .c_str();
#else
	return to_string(i);
#endif 
}



class MenuAbstract {
public:
	MenuAbstract() : name("Default") {}
	MenuAbstract(const char* name) : name(name) {}


	const char* name;
	virtual const char* getName() {
		return name;
	}

	virtual String getData() {
		return "";
	}

	virtual String getDataExtended() {
		return "";
	}

	bool hasSelection = false;
	bool hasAction = false;

	// UI interface for each menu item
	virtual void left() {}
	virtual void right() {}
	virtual void up() {}
	virtual void down() {}
	virtual void press() {}
	virtual void inc() {}
	virtual void dec() {}

	enum { maxLines = 14 };

};

MenuAbstract baseMenu;

class MenuBool : public MenuAbstract {
public:
	MenuBool(const char * name, bool *v) : MenuAbstract(name), val(v) {
		hasAction = true;
	}

	const char* getName() {
		//String r = name;
		//r += getData();
		//return r.c_str();
		return name;
	}

	void press() {
		*val = !*val;
	}

	String getData() {
		if (*val)
			return "On";
		else return "Off";
	}

	void inc() {
		press();
	}

	void dec() {
		press();
	}

	bool *val;
};





class MenuTextSelection : public MenuAbstract {
public:
	MenuTextSelection() : MenuAbstract("Custom text") {
		hasSelection = true;
		index = counter++;
	}

	static uint8_t counter;
	uint8_t index;

	String getData() {
		return intToString(index + 1);
	}

	String getDataExtended() {
		String r = msg;
		r += "\n";
		for (uint8_t i = 0; i < sel; i++) {
			r += " ";
		}
		r += "^";
		//static uint8_t tick = 0;
		//tick++;
		//if (tick > 4)
		//	r[sel] = 178;
		//if (tick > 6)
		//	tick = 0;
		return r;
	}

	void inc() {
		if (msg[sel] >= 'A' and msg[sel] < 'Z') {
			// upper case
			msg[sel]++;
		}
		else if (msg[sel] >= 'a' and msg[sel] < 'z') {
			//lower case
			msg[sel]++;
		}
		else if (msg[sel] == 'Z') {
			msg[sel] = 'a';
		}
		else if (msg[sel] == 'z') {
			msg[sel] = '!';
		}
		else if (msg[sel] == '!') {
			msg[sel] = '?';
		}
		else {
			msg[sel] = 'A';
		}
	}

	void dec() {
		if (msg[sel] > 'A' and msg[sel] <= 'Z') {
			// upper case
			msg[sel]--;
		}
		else if (msg[sel] > 'a' and msg[sel] <= 'z') {
			//lower case
			msg[sel]--;
		}
		else if (msg[sel] == 'A') {
			msg[sel] = ' ';
		}
		else if (msg[sel] == '?') {
			msg[sel] = '!';
		}
		else if (msg[sel] == '!') {
			msg[sel] = 'z';
		}
	}

	void up() {
		if (msg[sel]  >= 'A' and msg[sel] <= 'Z') msg[sel] += 32;
	}

	void down() {
		if (msg[sel] >= 'a' and msg[sel] <= 'z') msg[sel] -= 32;
	}

	void left() {
		if (sel > 0) sel--;
	}

	void right() {
		if (sel < maxChar) sel++;
	}

	void press() {
		char *toAddress;
		if (index == 0)
			toAddress = Data::custom1;
		if (index == 1)
			toAddress = Data::custom2;
		for (uint8_t i = 0; i < 15; i++) {
			toAddress[i] = msg[i];
		}
	}

	static const uint8_t maxChar = 11;
	char msg[maxChar+1] = "           ";
	uint8_t sel = 0;
	
	const uint8_t charUpperMin = 'A';
	const uint8_t charUpperMax = 'Z';
	const uint8_t charLowerMin = 'a';
	const uint8_t charLowerMax = 'z';

};

uint8_t MenuTextSelection::counter = 0;

class MenuCurrentPattern : public MenuAbstract {
public:
	MenuCurrentPattern() : MenuAbstract("") {}

	const char* getName() {
		// String r = "Current pattern:";
		// r += 
		return patterns.getCurrentPatternName();//r.c_str();
	}

	void inc() {
		patterns.inc();
	}
	void dec() {
		patterns.dec();
	}
};

class MenuTrigger : public MenuAbstract {
public:
	MenuTrigger() : MenuAbstract("Osc Type") {
		hasSelection = true;
	}

	String getData() {
#if defined(ESP32) || defined(CORE_TEENSY)
		return String(_Pattern::beat.getType());// .c_str();
#else
		return to_string(_Pattern::beat.getType());
#endif 
	}
	String getDataExtended() {
		String r = "";
		for (uint8_t i = 0; i < _Pattern::beat.numTypes() + 1; i++) {
			if (i == _Pattern::beat.getType())
				r += "+";
			else if (i == selected)
				r += ">";
			else
				r += " ";

			if (i < _Pattern::beat.numTypes())
				r += _Pattern::beat.getTriggerName(i);
			else
				r += "Back";
			r += "\n";
		}
		return r;
	}
	void up() {
		if (selected <= _Pattern::beat.numTypes())
			selected++;
	}
	void down() {
		if (selected > 0)
			selected--;
	}

	void inc() {
		const uint8_t currentType = _Pattern::beat.getType();
		if (currentType < _Pattern::beat.numTypes() + 1) {
			_Pattern::beat.setType(currentType + 1);
		}
	}
	void dec() {
		const uint8_t currentType = _Pattern::beat.getType();
		if (currentType > 0) {
			_Pattern::beat.setType(currentType - 1);
		}
	}

	void press() {
		if (selected < _Pattern::beat.numTypes())
			_Pattern::beat.setType(selected);
	}

	uint8_t selected = _Pattern::beat.OFF;

};



class MenuCurrentPalette : public MenuAbstract {
public:
	MenuCurrentPalette() : MenuAbstract("") {}

	const char* getName() {
		return paletteNames[(*paletteIndex_t)];
	}

	void inc() {
		paletteIndex_t.inc();
	}

	void dec() {
		paletteIndex_t.dec();
	}
};

//class MenuVariableNamed : public MenuVariable {
//public:
//	MenuVariableNamed() {}
//	MenuVariableNamed(VariableReference *var, const char* names, uint8_t len) : MenuVariable(var) {
//		hasSelection = true;
//	}
//
//	String getData() {
//		return var->getValue();
//	}
//};

class MenuVariable : public MenuAbstract {
public:
	MenuVariable() {}
	MenuVariable(VariableReference *var) : MenuAbstract(var->name), var(var), names(NULL), len(0) {}
	MenuVariable(VariableReference *var, const char** names, uint8_t len) : MenuAbstract(var->name), var(var), names(names), len(len) {
		hasSelection = true;
	}

	const char* getName() {
		if (var)
			return var->name;
	}


	virtual String getData() {
		if (var)
			return var->getValue();
	}

	String getDataExtended() {
		if (len == 0 or names == NULL) return "";
		String r = "";
		for (uint8_t i = 0; i < var->max + 1; i++) {
			if (i == *(*var))
				r += "x ";
			else if (i == selected)
				r += "> ";
			else
				r += "  ";

			if (i < var->max)
				r += names[i];
			else
				r += "Back";
			r += "\n";
		}
		return r;
	}

	void inc() {
		increaseRotationSpeed(1);
		var->inc(incAmount);
	}
	void dec() {
		increaseRotationSpeed(-1);
		var->dec(incAmount);
	}

	unsigned long lastUpdate = 0;
	int8_t lastDirection = 0;
	uint8_t incAmount = 1;
	uint8_t incCounter = 0;
	void increaseRotationSpeed(int8_t dir) {
		unsigned long now = GET_MILLIS();
		if (now - lastUpdate < 200) { // last update was within a certain time period
			if (lastDirection == dir) {//moving in same direction
				incCounter++;
				if (incCounter > 5)
				{
					//incAmount++;
					//incCounter = 0;
					if (incAmount < 5)
						incAmount++;
				}
			}
			else {
				lastDirection = dir;
				incAmount = 1;
				incCounter = 0;
			}

		}
		else { // time out for update
			lastDirection = 0;
			incAmount = 1;
			incCounter = 0;
		}
		lastUpdate = now; // reset time
	}

protected:
	VariableReference *var;
	const char **names;
	uint16_t len;
	uint8_t selected = 0;



};

class MenuAction : public MenuAbstract {
public:
	MenuAction() {}
	MenuAction(const char * name) : MenuAbstract(name) {
		//hasSelection = true;
		hasAction = true;
	}

	virtual String getData() {
		return "";
	}

	//virtual String getDataExtended() {
	//	return "$PROMPT";
	//}

	void press() {
		if (callback)
			callback();
	}


	void setCallback(void(*fn)()) { callback = fn; }
	void(*callback)() = NULL;


};


//takes const char*[] to array of item Names
//takes len given array length
//class MenuVariableNamed : public MenuVariable {
//public:
//	MenuVariableNamed() {}
//	MenuVariableNamed(VariableReference *var, const char** names, uint8_t len) : MenuVariable(var), names(names), len(len) {
//		hasSelection = true;
//	}
//
//	String getData() {
//		return var->getValue();
//	}
//
//	String getDataExtended() {
//		String r = "";
//		for (uint8_t i = 0; i < var->max + 1; i++) {
//			if (i == *(*var))
//				r += "x ";
//			else if (i == selectedIndex)
//				r += "> ";
//			else
//				r += "  ";
//
//			if (i < var->max)
//				r += names[i];
//			else
//				r += "Back";
//			r += "\n";
//		}
//		return r;
//	}
//
//	void up() {
//		if (selected <= var->max)
//			selectedPattern++;
//		//selectedPattern = (selectedPattern + 1) % (patterns.numPatterns + 1); //plus one for back button
//	}
//	void down() {
//		if (selected > 0)
//			selectedPattern--;
//		//selectedPattern = (selectedPattern - 1 + (patterns.numPatterns+1)) % (patterns.numPatterns+1);
//	}
//
//	void inc() {
//		up();
//	}
//	void dec() {
//		down();
//	}
//
//	void press() {
//		if (selected < var->max)
//			patterns.set(selected);
//	}
//
//	uint8_t selectedIndex = 0;
//	const char **names;
//	uint8_t len;
//};

class MenuTextList : public MenuAbstract {
public: 
	MenuTextList() :MenuAbstract("Text list") {
		hasSelection = true;
	}

	String getDataExtended() {
		String r = "";
		//for (uint8_t i = 0; i < paletteIndex_t.max + 1; i++) {
		for (uint8_t i = frame; i < frame + maxLines; i++) {

			if (i == *Data::textIndex_t)
				r += "+";
			else if (i == selected)
				r += ">";
			else
				r += " ";

			if (i < Data::textIndex_t.max)
				r += Data::textOptions[i];
			else if (i == Data::textIndex_t.max)
				r += "Back";

			r += "\n";
		}
		if (frame + maxLines <= Data::textIndex_t.max) {
			if (*Data::textIndex_t > frame + maxLines) r += "x ...";
			else r += "  ...";
		}

		return r;
	}

	void up() {
		if (selected < Data::textIndex_t.max) selected++;
		if (selected >= maxLines + frame) frame++;

	}
	void down() {
		if (selected > 0) selected--;
		if (selected < frame) frame--;

	}
	void inc() { Data::textIndex_t.inc(); }
	void dec() { Data::textIndex_t.dec(); }

	void press() {
		if (selected < Data::textIndex_t.max)
			*Data::textIndex_t = selected;
	}


	uint8_t selected = 0;
	uint8_t frame = 0;

};

class MenuPaletteList : public MenuAbstract {
public:
	MenuPaletteList() : MenuAbstract("Palette List") {
		hasSelection = true;
	}

	String getData() {
		//return getPaletteName(*paletteIndex_t);
#if defined(ESP32) || defined(CORE_TEENSY)
		return String(*paletteIndex_t);// .c_str();
#else
		return to_string(*paletteIndex_t);
#endif 
	}

	String getDataExtended() {
		String r = "";
		//for (uint8_t i = 0; i < paletteIndex_t.max + 1; i++) {
		for (uint8_t i = frame; i < frame + maxLines; i++) {

			if (i == *paletteIndex_t)
				r += "+";
			else if (i == selected)
				r += ">";
			else
				r += " ";

			if (i <= paletteIndex_t.max)
				r += getPaletteName(i);
			else if (i == paletteIndex_t.max+1)
				r += "Back";

			r += "\n";
		}
		if (frame + maxLines <= paletteIndex_t.max+1) {
			if (*paletteIndex_t > frame + maxLines) r += "x ...";
			else r += "  ...";
		}
		if (callback) {
			callback(selected);
		}
		return r;
	}

	void up() {
		if (selected <= paletteIndex_t.max) selected++;
		if (selected >= maxLines + frame) frame++; 
		
	}
	void down() {
		if (selected > 0) selected--;
		if (selected < frame) frame--;

	}
	void inc() { paletteIndex_t.inc(); }
	void dec() { paletteIndex_t.dec(); }

	void press() {
		if (selected <= paletteIndex_t.max)
			*paletteIndex_t = selected;
	}


	void setCallback(void(*fn)(uint8_t)) { callback = fn; }
	void(*callback)(uint8_t) = NULL;

	uint8_t selected = 0;
	uint8_t frame = 0;

};

class MenuPaletteListHb : public MenuAbstract {
public:
	MenuPaletteListHb() : MenuAbstract("Palette List") {
		hasSelection = true;
	}

	String getData() {
		//return getPaletteName(*paletteIndexHb_t);
#if defined(ESP32) || defined(CORE_TEENSY)
		return String(*paletteIndexHb_t);// .c_str();
#else
		return to_string(*paletteIndexHb_t);
#endif 
	}

	String getDataExtended() {
		String r = "";
		//for (uint8_t i = 0; i < paletteIndex_t.max + 1; i++) {
		for (uint8_t i = frame; i < frame + maxLines; i++) {

			if (i == *paletteIndexHb_t)
				r += "+";
			else if (i == selected)
				r += ">";
			else
				r += " ";

			if (i <= paletteIndexHb_t.max)
				r += getPaletteName(i);
			else if (i == paletteIndexHb_t.max+1)
				r += "Back";

			r += "\n";
		}
		if (frame + maxLines <= paletteIndexHb_t.max+1) {
			if (*paletteIndexHb_t > frame + maxLines) r += "x ...";
			else r += "  ...";
		}
		return r;
	}

	void up() {
		if (selected <= paletteIndexHb_t.max) selected++;
		if (selected >= maxLines + frame) frame++;
	}
	void down() {
		if (selected > 0) selected--;
		if (selected < frame) frame--;
	}
	void inc() { paletteIndexHb_t.inc(); if (callback)	callback(); }
	void dec() { paletteIndexHb_t.dec(); if (callback)	callback();	}

	void press() {
		if (selected <= paletteIndexHb_t.max)
			*paletteIndexHb_t = selected;
		if (callback)
			callback();
	}

	void setCallback(void(*fn)()) { callback = fn; }
	void(*callback)() = NULL;


	uint8_t selected = 0;
	uint8_t frame = 0;

};


class MenuPatternList : public MenuAbstract {
public:
	MenuPatternList() : MenuAbstract("Pattern ") {
		hasSelection = true;
	}

	//const char* getName() {
	//	return patterns.getCurrentPatternName();//r.c_str();
	//}

	String getData() {
		//return patterns.getCurrentPatternName();
		//return "";
#if defined(ESP32) || defined(CORE_TEENSY)
		return String(Data::currentPattern);// .c_str();
#else
		return to_string(Data::currentPattern);
#endif 
	}

	String getDataExtended() {
		String r = "";
		//for (uint8_t i = 0; i < patterns.numPatterns + 1; i++) {
		for (uint8_t i = frame; i < frame + maxLines; i++) {
			//if (i == frame + maxLines - 1 and patterns.numPatterns >= i) {
			//	r += "...";
			//}

			if (i == Data::currentPattern)
				r += "+";
			else if (i == selectedPattern)
				r += ">";
			else
				r += " ";

			//if (i < frame + maxLines) {
			//	if (i < patterns.numPatterns)
			//		r += patterns.getPatternName(i);
			//	else if (i == patterns.numPatterns)
			//		r += "Back";
			//}
			//else {
			//	// last item 
			//	//if (i <= patterns.numPatterns) {
			//	//	r += "...";
			//	//}
			//}

			if (i < patterns.numPatterns)
				r += patterns.getPatternName(i);
			else if ( i == patterns.numPatterns)
				r += "Back";
			
			r += "\n";
		}

		if (frame + maxLines <= patterns.numPatterns) {
			if (Data::currentPattern > frame+maxLines) r += "x ...";
			else r += "  ...";
		}
		return r;
	}

	uint8_t selectedPattern = Data::currentPattern;
	uint8_t frame = 0;


	
	void up() {
		if (selectedPattern < patterns.numPatterns)
			selectedPattern++;
		if (selectedPattern >= maxLines + frame) frame++;
		//selectedPattern = (selectedPattern + 1) % (patterns.numPatterns + 1); //plus one for back button
	}
	void down() {
		if (selectedPattern > 0)
			selectedPattern--;
		if (selectedPattern < frame) frame--;
		//selectedPattern = (selectedPattern - 1 + (patterns.numPatterns+1)) % (patterns.numPatterns+1);
	}

	void inc() {
		//up();
		patterns.inc();
	}
	void dec() {
		//down();
		patterns.dec();
	}

	void press() {
		if (selectedPattern < patterns.numPatterns)
			patterns.set(selectedPattern);
	}


};

class MenuPatternListHb : public MenuAbstract {
public:
	MenuPatternListHb() : MenuAbstract("Pattern ") {
		hasSelection = true;
		maxI = frame + maxLines;
		if (maxI > Data::hbMaxPatterns) maxI = Data::hbMaxPatterns;
	}

	//const char* getName() {
	//	return patterns.getCurrentPatternName();//r.c_str();
	//}

	String getData() {
		//return patterns.getCurrentPatternName();
		return "";
#if defined(ESP32) || defined(CORE_TEENSY)
		return String(Data::currentPattern);// .c_str();
#else
		return to_string(Data::currentPattern);
#endif 
	}

	String getDataExtended() {
		String r = "";
		for (uint8_t i = frame; i < maxI; i++) {
			if (i == Data::hbCurrentPattern)
				r += "+";
			else if (i == selectedPattern)
				r += ">";
			else
				r += " ";

			if (i < Data::hbMaxPatterns)
				r += Data::hbPatternNames[i];
			else if (i == Data::hbMaxPatterns)
				r += "Back";

			r += "\n";
		}

		if (frame + maxLines <= Data::hbMaxPatterns) {
			if (Data::hbCurrentPattern > maxI) r += "x ...";
			else r += "  ...";
		}
		return r;
	}

	uint8_t selectedPattern = Data::hbCurrentPattern;
	uint8_t frame = 0;
	uint8_t maxI = 0;


	void up() {
		if (selectedPattern < Data::hbMaxPatterns)
			selectedPattern++;
		if (selectedPattern >= maxI) frame++;
		//selectedPattern = (selectedPattern + 1) % (patterns.numPatterns + 1); //plus one for back button
	}
	void down() {
		if (selectedPattern > 0)
			selectedPattern--;
		if (selectedPattern < frame) frame--;
		//selectedPattern = (selectedPattern - 1 + (patterns.numPatterns+1)) % (patterns.numPatterns+1);
	}

	void inc() {
		if (Data::hbCurrentPattern < Data::hbMaxPatterns -1)
			Data::hbCurrentPattern++;
		if (callback)
			callback();
	}
	void dec() {
		if (Data::hbCurrentPattern > 0)
			Data::hbCurrentPattern--;
		if (callback)
			callback();
	}

	void press() {
		if (selectedPattern < Data::hbMaxPatterns) {
			Data::hbCurrentPattern = selectedPattern;
			if (callback)
				callback();
		}
	}

	void setCallback(void(*fn)()) { callback = fn; }
	void(*callback)() = NULL;
};



/*
template<class ITEM>
class MenuDisplay {
public:
	MenuDisplay() : MenuAbstract("Display") {}
	MenuDisplay(const char* name, ITEM &ref) : MenuAbstract(name), data(ref) {}


};



template<class ITEM>
class MenuList {
public:
	MenuList() : {}


};

template<class ITEM>
class MenuItem : public MenuAbstract {
public:
	MenuItem() : {}
	MenuItem(const char* name, ITEM &ref) : MenuAbstract(name), data(ref) {}

	ITEM &data;
	ITEM getData() {
		return data;
	}


};

 */


class MenuPage {
public:
	MenuPage() : name("Default page") {}
	MenuPage(const char* name) : name(name) {}

	const char* name;
	const char* getName() {
		return name;
	}

	virtual const char* getItemName(size_t i) {
		return "";
	}

	virtual String getItemData(size_t i) {
		return "";
	}

	String getPageData() {
		String r = "";

		if (selected) {
			r += getItemName(currentItem);
			r += "\n";
			r += getItemData(currentItem);
			r += "\n";
		}
		else {
			for (size_t i = 0; i < getNumItems(); i++) {
				if (i == currentItem) {
					r += ">";
				}
				else {
					r += " ";
				}
				r += getItemName(i);
				r += " ";
				r += getItemData(i);
				r += "\n";
			}
		}
		return r;
	}

	virtual size_t getNumItems() {
		return 0;
	}

	virtual MenuAbstract *getCurrentItem() {
		return nullptr;
	}
	size_t currentItem = 0;

	// if an item is selected, can display a seperate page with details for it
	bool selected = false;

	virtual void left() { getCurrentItem()->left(); }
	virtual void right() { getCurrentItem()->right(); }
	virtual void up() {
		if (selected)
			getCurrentItem()->up();
		else
			currentItem = (currentItem + 1) % getNumItems();
	}
	virtual void down() {
		if (selected)
			getCurrentItem()->down();
		else
			currentItem = (currentItem - 1 + getNumItems()) % getNumItems();
	}

	void press() {
		if (getCurrentItem()->hasSelection) {
			if (selected)
				getCurrentItem()->press(); // only press when leaving selection
			selected = !selected;
		}
		else if (getCurrentItem()->hasAction)
			getCurrentItem()->press();
	}
	void inc() {
		if (selected and getCurrentItem()->getName() != "Custom text")
			up();
		else getCurrentItem()->inc();
	}
	void dec() {
		if (selected and getCurrentItem()->getName() != "Custom text")
			down();
		else getCurrentItem()->dec();
	}
};



class PagePattern : public MenuPage {
public:
	PagePattern() : MenuPage("Patterns") {
		TapTempo.setCallback([]() {
			_Pattern::beat.sync();
		});
		tempo_t.setCallback([]() {
			_Pattern::beat.setBPM(_Pattern::beat.bpmVal << 8);
		});
	}
	
	size_t getNumItems() {
		return PagePattern::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems) {
			return (items[i]->getName());
		}
		return "";
	}

	String getItemData(size_t i) {
		if (i < numItems) {
			if (selected)
				return (items[i]->getDataExtended());
			else
				return (items[i]->getData());
		}
		return "";
	}

	MenuAbstract *getCurrentItem() {
		return items[currentItem];
	}

	MenuCurrentPattern CurrentPattern;
	MenuPatternList PatternList;
	MenuVariable Brightness{ &Data::brightness_t };
	
	
	
	VariableReference tempo_t{ "Tempo", &_Pattern::beat.bpmVal, 120, 30,240 };
	MenuVariable Tempo{ &tempo_t };
	MenuAction TapTempo{ "Tap tempo" };
	MenuTrigger OscType;

	VariableReference audioThreshold_t{ "Audio thres", &Data::audioThreshold, 128, 0, 255 };
	VariableReference noiseFloor_t{ "Noise floor", &Data::noiseFloor, 65, 0, 255 };
	MenuVariable AudioThreshold{ &audioThreshold_t };
	MenuVariable NoiseFloor{ &noiseFloor_t };

	static const size_t numItems = 8;
	MenuAbstract *items[numItems] = {
		&PatternList,
		&CurrentPattern,
		&Brightness,
		&Tempo,
		&TapTempo,
		&OscType,
		&AudioThreshold,
		&NoiseFloor

	};




};

class PageColour : public MenuPage {
public:
	PageColour() : MenuPage("Colour") {}


	size_t getNumItems() {
		return PageColour::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems) {
			return (items[i]->getName());
		}
		return "";
	}

	String getItemData(size_t i) {
		if (i < numItems) {
			if (selected)
				return (items[i]->getDataExtended());
			else
				return (items[i]->getData());
		}
		return "";
	}

	MenuAbstract *getCurrentItem() {
		return items[currentItem];
	}

	VariableReference blendTime_t{ "Blend amount", &gfx.IncAmount, 12, 1, 64 };
	VariableReference hueSpeed_t{ "Hue speed", &Data::hueSpeed, 50, 10, 200 };
	VariableReference paletteSpeed_t{ "Palette change", &Data::changePaletteDelay, 0, 0, 120 };


	//menu items
	MenuCurrentPalette	CurrentPalette;
	MenuPaletteList		PaletteList;
	MenuBool			HueChange{ "Hue change", &Data::hueChange };
	MenuVariable 		HueSpeed{ &hueSpeed_t};
	MenuVariable		PaletteSpeed{ &paletteSpeed_t};
	MenuVariable		BlendTime{ &blendTime_t };

	static const size_t numItems = 6;
	MenuAbstract *items[numItems] = {
		&PaletteList,
		&CurrentPalette,
		&PaletteSpeed,
		&BlendTime,
		&HueChange,
		&HueSpeed
		
	};


};

class PageEffects : public MenuPage {
public:
	PageEffects() : MenuPage("Effects") {
		TextSingleShot.setCallback([]() {
			patterns.blinkText();
		});
	}


	size_t getNumItems() {
		return PageEffects::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems)
			return items[i]->getName();
		return "";
	}

	String getItemData(size_t i) {
		if (i < numItems) {
			if (selected)
				return (items[i]->getDataExtended());
			else
				return (items[i]->getData());
		}
		return "";
	}

	MenuAbstract *getCurrentItem() {
		return items[currentItem];
	}

	VariableReference blurFx_t{ "Blur", &_Pattern::blurFx };
	VariableReference fadeFx_t{ "Fade", &_Pattern::fadeFx };
	VariableReference spiralFx_t{ "Spiral", &_Pattern::spiralFx, 128, 108, 148 };
	VariableReference glitterFx_t{ "Glitter", &_Pattern::glitterFx, 0, 0, 10 };
	VariableReference textOpacity_t{ "Text opacity", &gfx.textOpacity };
	

	MenuBool TextOn{ "Text: ", &Data::textOn };
	MenuVariable TextOpacity{ &textOpacity_t };
	MenuAction TextSingleShot{ "Blink once" };
	
	MenuTextList TextList;
	MenuTextSelection TextSelection1;
	MenuTextSelection TextSelection2;
	
	MenuBool CustomOn{ "Effects: ", &_Pattern::useCustomEffect };
	MenuVariable BlurFx{ &blurFx_t };
	MenuVariable FadeFx{ &fadeFx_t };
	MenuVariable SpiralFx{ &spiralFx_t };
	MenuVariable GlitterFx{&glitterFx_t};



	

	//menu items
	static const size_t numItems = 10;
	MenuAbstract *items[numItems] = {
		&TextOn,
		&TextOpacity,
		&TextSingleShot,
		&TextList,
		&TextSelection1,
		&TextSelection2,
		&CustomOn,
		&BlurFx, 
		&FadeFx,
		&SpiralFx,
		&GlitterFx
		
	};


};


class PageHeadbands : public MenuPage {
public:
	PageHeadbands() : MenuPage("Headbands") {}

	size_t getNumItems() {
		return PageHeadbands::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems)
			return items[i]->getName();
		return "";
	}

	String getItemData(size_t i) {
		if (i < numItems) {
			if (selected)
				return (items[i]->getDataExtended());
			else
				return (items[i]->getData());
		}
		return "";
	}

	MenuAbstract *getCurrentItem() {
		return items[currentItem];
	}

	MenuPatternListHb Pattern;
	MenuPaletteListHb Palette;
	MenuVariable Brightness{ &Data::brightnessHb_t };
	MenuVariable Fade{ &Data::fadeHb_t };
	MenuVariable Speed{ &Data::speedHb_t };
	MenuVariable HueDelay{ &Data::hueDelayHb_t };
	MenuAction Sync{ "Sync" };
	MenuAction Settings{ "Push Settings" };

	MenuAction Trigger{ "Trigger" };


	//menu items
	static const size_t numItems = 9;
	MenuAbstract *items[numItems] = {
		&Pattern,
		&Palette,
		&Brightness,
		&Fade, 
		&Speed,
		&HueDelay,
		&Sync,
		&Settings,
		&Trigger
	};


};

class PageSettings : public MenuPage {
public:
	PageSettings() : MenuPage("Settings") {
		temperature_t.setCallback(&PageSettings::setColorTemp);
	}

	size_t getNumItems() {
		return PageSettings::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems)
			return items[i]->getName();
		return "";
	}

	String getItemData(size_t i) {
		if (i < numItems) {
			if (selected)
				return (items[i]->getDataExtended());
			else
				return (items[i]->getData());
		}
		return "";
	}

	MenuAbstract *getCurrentItem() {
		return items[currentItem];
	}


	static void setColorTemp() {
		gfx.setColorTemp(colourTempValues[colorTemp]);
	}

	static uint8_t colorTemp;
	static const uint32_t colourTempValues[20];


	VariableReference temperature_t{ "Color Temp", &colorTemp, 0 ,0, 19 };


	MenuVariable Temperature{ &temperature_t};
	MenuVariable Backlight{ &Data::backlight_t };
	MenuAction Save{ "Save Settings" };
	MenuAction Load{ "Load Settings" };


	//menu items
	static const size_t numItems = 4;
	MenuAbstract *items[numItems] = {
		&Temperature,
		&Backlight,
		&Save,
		&Load
	};


};

uint8_t PageSettings::colorTemp = 6;

const uint32_t PageSettings::colourTempValues[20] = {
		Candle, //0
		Tungsten40W,
		Tungsten100W,
		Halogen,
		CarbonArc,
		HighNoonSun, //5
		DirectSunlight,
		OvercastSky,
		ClearBlueSky,
		WarmFluorescent,
		StandardFluorescent, //10
		CoolWhiteFluorescent,
		FullSpectrumFluorescent,
		GrowLightFluorescent,
		BlackLightFluorescent,
		MercuryVapor, //15
		SodiumVapor,
		MetalHalide,
		HighPressureSodium,
		UncorrectedTemperature //
};




class Menu {
public:
	Menu() {}

	PagePattern		Pattern;
	PageColour		Colour;
	PageEffects		Effects;
	PageHeadbands	Headbands;
	PageSettings	Settings;

	static const size_t numPages = 5;
	size_t currentPageIndex = 0;
	MenuPage *pages[numPages] = {
		&Pattern,
		&Colour,
		&Effects,
		&Headbands,
		&Settings
	};

	void nextPage() {
		currentPageIndex = (currentPageIndex + 1) % numPages;
	}

	void previousPage() {
		currentPageIndex = (currentPageIndex - 1 + numPages) % numPages;
	}

	// 
	const char* getPageName(size_t i) {
		if (i >= numPages)
			return "";
		return pages[i]->getName();
	}

	String getPageData(size_t i) {
		if (i > numPages)
			return "";
		String r;
		for (size_t j = 0; j < (pages[i]->getNumItems()); j++) {
			if (j == pages[i]->currentItem) {
				r += ">";
			}
			else {
				r += " ";
			}
			r += pages[i]->getItemName(j);
			r += " ";
			r += pages[i]->getItemData(j);
			r += "\n";

		}
		return r;
	}

	MenuPage *currentPage() {
		return pages[currentPageIndex];
	}

	//String getCurrentPageNames() {
	//	String r;
	//	for (size_t i = 0; i < currentPage()->count(); i++) {
	//		r.append(currentPage()->getItem(i));
	//	}
	//	return r;
	//}

	size_t numItemsCurrentPage() {
		return currentPage()->getNumItems();
	}

	void left() {
		//if (currentPage()->getCurrentItem() == Effects.TextSelection)
		if (currentPage()->getCurrentItem()->getName() == "Custom text" and currentPage()->selected) {
			currentPage()->left();
		}
		else {
			previousPage();
		}
	}
	void right() {
		if (currentPage()->getCurrentItem()->getName() == "Custom text" and currentPage()->selected) {
			currentPage()->right();
		}
		else {
			nextPage();
		}
	}
	void up() {
		currentPage()->up();
	}
	void down() {
		currentPage()->down();
	}
	void press() {
		currentPage()->press();
	}
	void inc() {
		currentPage()->inc();
	}
	void dec() {
		currentPage()->dec();
	}

	void display() {

		String r = currentPage()->getName();
		// log current page name
		r = currentPage()->getPageData();
		//log page data
		// return r;
		
	}
};

Menu menu;
