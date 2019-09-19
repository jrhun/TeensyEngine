#pragma once




#include <vector>
#include "../data.h"
#include "Patterns/PatternController.h"

#if defined(ESP32) || defined(CORE_TEENSY)
#include "FastLED.h"
#else 
#include "FastLED_PC.h"
#endif
// menu

template<class TYPE>
class VariableContainer {
public:
	VariableContainer() : min(0), max(0), wrap(true) {}
	VariableContainer(const char* name, TYPE &t, TYPE min, TYPE max, bool wrap = false) : name(name), d(t), min(min), max(max), wrap(wrap) {}

	const char* name;
	const char* getName() {
		return name;
	}

	TYPE &d;
	TYPE min;
	TYPE max;
	bool wrap;

	void inc(TYPE amount = 1) {
		d += amount;
		if (d > max) {
			if (wrap) 	d -= max;
			else 		d = max;
		}
	}

	void dec(TYPE amount = 1) {
		if (d - amount < min) {
			if (wrap) 	d = d - amount + max; // works for both signed/unsigned + floats
			else		d = min;
		}
		else			d -= amount;
	}

	void oscilate();
	void trigger();



};

typedef VariableContainer<float>		FloatContainer;
typedef VariableContainer<int>			IntContainer;
typedef VariableContainer<unsigned int> uIntContainer;
typedef VariableContainer<int8_t>		Int8Container;
typedef VariableContainer<uint8_t>		uInt8Container;


template<class T>
class VariableControl {
public:
	VariableControl() : {}

	VariableControl(VariableContainer<T> &var) : var(var), min(var.min), max(var.max) {
		delta = max - min;
		triggerEnd = min;
	}

	VariableControl(VariableContainer<T> &var, T _min, T _max) : var(var), min(_min), max(_max) {
		if (_min < var.min)
			min = var.min;
		if (_max > var.max)
			max = var.max;
		delta = max - min;
		triggerEnd = min;
	}

	VariableContainer<T> &var;
	T min;
	T max;
	T delta;
	T triggerEnd;

	bool oscilateOn = false;
	bool rampOn = false;
	bool triggerOn = false;

	unsigned long lastUpdate;
	void update() {
		unsigned long now = GET_MILLIS();
		if ((oscilateOn || rampOn || triggerOn) and (now - lastUpdate > interval)) {
			//lastUpdate += interval;
			lastUpdate = now;

			if (oscilateOn) {
				static bool dirUp = true;
				bool w = var.wrap;
				var.wrap = false;
				if (dirUp) {
					var.inc();
					if (var.d >= max)
						dirUp = false;
				}
				else {
					var.dec();
					if (var.d <= min)
						dirUp = true;
				}

				var.wrap = w;
			}

			else if (rampOn) {
				bool w = var.wrap;
				var.wrap = true;
				var.inc();
				var.wrap = w;
			}

			else if (triggerOn) {
				bool w = var.wrap;
				var.wrap = false;
				var.dec();
				if (var.d <= triggerEnd) {
					triggerOn = false;
					var.d = triggerEnd;
				}
				var.wrap = w;
			}
		}
	}

	float interval = 10;

	void oscilate(accum88 BPM) {
		float period = (60000.0f / (BPM / 256.0f)); // e.g. BPM = 100, delta is 100, period = 600 
		interval = period / (delta * 2.0f);  // then interval = 3
		oscilateOn = true;
		rampOn = false;
		trigger = false;
	}

	void ramp(accum88 BPM) {
		float period = (60000.0f / (BPM / 256.0f)); // e.g. BPM = 100, delta is 100, period = 600 
		interval = period / float(delta);  // then interval = 3
		rampOn = true;
		oscilateOn = false;
		triggerOn = false;
	}

	// triggers a sweep from max to current value (or min)
	// length in ms of sweep
	void trigger(unsigned long length = 200, T _max = max) {
		interval = round(float(length) / float(delta)); // = 2 if length = 200 and max - min = 100

		triggerEnd = var.d;
		var.d = _max;

		triggerOn = true;
		rampOn = false;
		oscilateOn = false;
	}
};


class MenuAbstract {
public:
	MenuAbstract() : name("Default") {}
	MenuAbstract(const char* name) : name(name) {}


	const char* name;
	virtual const char* getName() {
		return name;
	}

	virtual const char* getData() {
		return "";
	}

	// UI interface for each menu item
	virtual void left() {}
	virtual void right() {}
	virtual void up() {}
	virtual void down() {}
	virtual void press() {}
	virtual void inc() {}
	virtual void dec() {}

};

MenuAbstract default;

class MenuCurrentPattern : public MenuAbstract {
public: 
	MenuCurrentPattern() : MenuAbstract("") {}

	const char* getName() {
		return patterns.getCurrentPatternName();
	}
};

class MenuCurrentPalette : public MenuAbstract {
public:
	MenuCurrentPalette() : MenuAbstract("") {}

	const char* getName() {
		return "palette temp";
	}
};


class MenuPatternList : public MenuAbstract {
public: 
	MenuPatternList() : MenuAbstract("Change pattern") {}

	const char* getData() {
		return to_string(Data::currentPattern).c_str();
	}

	void inc() {
		patterns.inc();
	}
	void dec() {
		patterns.dec();
	}
};



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

	virtual const char* getItemData(size_t i) {
		return "";
	}
	
	virtual size_t getNumItems() {
		return 0;
	}

	virtual size_t getCurrentItem() {
		return 0;
	}

	// no UI interface for page? just passes through to items
	virtual void left() {}
	virtual void right() {}
	virtual void up() {}
	virtual void down() {}
	virtual void press() {}
	virtual void inc() {}
	virtual void dec() {}
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

	const char* getItemData(size_t i) {
		if (i < numItems) {
			return (items[i]->getData());
		}
		return "";
	}

	size_t getCurrentItem() {
		return currentItem;
	}

	MenuCurrentPattern CurrentPattern;
	MenuPatternList PatternList;

	static const size_t numItems = 2;
	size_t currentItem = 0;
	MenuAbstract *items[numItems] = {
		&CurrentPattern,
		&PatternList,
	};




};

class PageColour : public MenuPage {
public:
	PageColour() : MenuPage("Colour") {}


	size_t getNumItems() {
		return PagePattern::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems) {
			return (items[i]->getName());
		}
		return "";
	}

	const char* getItemData(size_t i) {
		if (i < numItems)
			return items[i]->getData();
		return "";
	}

	size_t getCurrentItem() {
		return currentItem;
	}


	//menu items
	MenuCurrentPalette CurrentPalette;

	static const size_t numItems = 1;
	size_t currentItem = 0;
	MenuAbstract *items[numItems] = {
		&CurrentPalette
	};


};

class PageEffects : public MenuPage {
public:
	PageEffects() : MenuPage("Effects") {}


	size_t getNumItems() {
		return PagePattern::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems)
			return items[i]->getName();
		return "";
	}

	const char* getItemData(size_t i) {
		if (i < numItems)
			return items[i]->getData();
		return "";
	}

	size_t getCurrentItem() {
		return currentItem;
	}


	//menu items
	static const size_t numItems = 1;
	size_t currentItem = 0;
	MenuAbstract *items[numItems] = {
		&default
	};


};

class PageHeadbands : public MenuPage {
public:
	PageHeadbands() : MenuPage("Headbands") {}

	size_t getNumItems() {
		return PagePattern::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems)
			return items[i]->getName();
		return "";
	}

	const char* getItemData(size_t i) {
		if (i < numItems)
			return items[i]->getData();
		return "";
	}

	size_t getCurrentItem() {
		return currentItem;
	}


	//menu items
	static const size_t numItems = 1;
	size_t currentItem = 0;
	MenuAbstract *items[numItems] = {
		&default
	};


};

class PageSettings : public MenuPage {
public:
	PageSettings() : MenuPage("Settings") {}

	size_t getNumItems() {
		return PagePattern::numItems;
	}

	const char* getItemName(size_t i) {
		if (i < numItems)
			return items[i]->getName();
		return "";
	}

	const char* getItemData(size_t i) {
		if (i < numItems)
			return items[i]->getData();
		return "";
	}

	size_t getCurrentItem() {
		return currentItem;
	}


	//menu items
	static const size_t numItems = 1;
	size_t currentItem = 0;
	MenuAbstract *items[numItems] = {
		&default
	};


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

	// 
	const char* getPageName(size_t i) {
		if (i >= numPages)
			return "";
		return pages[i]->getName();
	}

	const char* getPageData(size_t i) {
		if (i >= numPages)
			return "";
		String r;
		for (size_t j = 0; j < (pages[i]->getNumItems()); j++) {
			if (j == pages[i]->getCurrentItem()) {
				r += ">";
			}
			else {
				r += " ";
			}
			r += pages[i]->getItemName(j);
			r += "/t";
			r += pages[i]->getItemData(j);
			r += "/n";

		}
		return r.c_str();
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

	}
	void right() {

	}
	void up() {

	}
	void down() {

	}
	void press() {

	}
	void inc() {

	}
	void dec() {

	}
};

Menu menu;