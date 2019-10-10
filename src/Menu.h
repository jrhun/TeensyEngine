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

};

MenuAbstract baseMenu;

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
	MenuTrigger() : MenuAbstract("Trigger") {
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
				r += "x";
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

	virtual String getData() {
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
	void increaseRotationSpeed(int8_t dir) {
		unsigned long now = GET_MILLIS();
		if (now - lastUpdate < 300) { // last update was within a certain time period
			if (lastDirection == dir) {//moving in same direction
				incAmount++;
				if (incAmount > 5)
					incAmount++;
			}
			else {
				lastDirection = dir;
			}

		}
		else { // time out for update
			lastDirection = 0;
			incAmount = 1;
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

class MenuPaletteList : public MenuAbstract {
public:
	MenuPaletteList() : MenuAbstract("Palette List") {
		hasSelection = true;
	}

	String getData() {
#if defined(ESP32) || defined(CORE_TEENSY)
		return String(*paletteIndex_t);// .c_str();
#else
		return to_string(*paletteIndex_t);
#endif 
	}

	String getDataExtended() {
		String r = "";
		for (uint8_t i = 0; i < paletteIndex_t.max + 1; i++) {
			if (i == *paletteIndex_t)
				r += "x ";
			else if (i == selected)
				r += "> ";
			else
				r += "  ";

			if (i < paletteIndex_t.max)
				r += getPaletteName(i);
			else
				r += "Back";
			r += "\n";
		}
		return r;
	}

	void up() {
		if (selected <= paletteIndex_t.max) selected++;
	}
	void down() {
		if (selected > 0) selected--;
	}
	void inc() { paletteIndex_t.inc(); }
	void dec() { paletteIndex_t.dec(); }

	void press() {
		if (selected < paletteIndex_t.max)
			*paletteIndex_t = selected;
	}

	uint8_t selected = 0;
};



class MenuPatternList : public MenuAbstract {
public:
	MenuPatternList() : MenuAbstract("Pattern list") {
		hasSelection = true;
	}

	String getData() {
		return "";
#if defined(ESP32) || defined(CORE_TEENSY)
		return String(Data::currentPattern);// .c_str();
#else
		return to_string(Data::currentPattern);
#endif 
	}

	String getDataExtended() {
		String r = "";
		for (uint8_t i = 0; i < patterns.numPatterns + 1; i++) {
			if (i == Data::currentPattern)
				r += "x ";
			else if (i == selectedPattern)
				r += "> ";
			else
				r += "  ";

			if (i < patterns.numPatterns)
				r += patterns.getPatternName(i);
			else
				r += "Back";
			r += "\n";
		}
		return r;
	}

	uint8_t selectedPattern = Data::currentPattern;


	void up() {
		if (selectedPattern <= patterns.numPatterns)
			selectedPattern++;
		//selectedPattern = (selectedPattern + 1) % (patterns.numPatterns + 1); //plus one for back button
	}
	void down() {
		if (selectedPattern > 0)
			selectedPattern--;
		//selectedPattern = (selectedPattern - 1 + (patterns.numPatterns+1)) % (patterns.numPatterns+1);
	}

	void inc() {
		up();
		//patterns.inc();
	}
	void dec() {
		down();
		//patterns.dec();
	}

	void press() {
		if (selectedPattern < patterns.numPatterns)
			patterns.set(selectedPattern);
	}
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

	virtual void left() {}
	virtual void right() {}
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
		getCurrentItem()->inc();
	}
	void dec() {
		getCurrentItem()->dec();
	}
};



class PagePattern : public MenuPage {
public:
	PagePattern() : MenuPage("Patterns") {}


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
	MenuTrigger Trigger;
	//MenuVariable Trigger{ &Data::triggerType_t };

	static const size_t numItems = 4;
	MenuAbstract *items[numItems] = {
		&CurrentPattern,
		&PatternList,
		&Brightness,
		&Trigger
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

	VariableReference blendTime_t{ "Blend steps", &gfx.IncAmount, 12, 1, 64 };

	//menu items
	MenuCurrentPalette	CurrentPalette;
	MenuPaletteList		PaletteList;
	MenuVariable		BlendTime{ &blendTime_t };

	static const size_t numItems = 3;
	MenuAbstract *items[numItems] = {
		&CurrentPalette,
		&PaletteList,
		&BlendTime
	};


};

class PageEffects : public MenuPage {
public:
	PageEffects() : MenuPage("Effects") {}


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


	//menu items
	static const size_t numItems = 1;
	MenuAbstract *items[numItems] = {
		&baseMenu
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


	//menu items
	static const size_t numItems = 1;
	MenuAbstract *items[numItems] = {
		&baseMenu
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
		previousPage();
	}
	void right() {
		nextPage();
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
