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
	MenuPatternList() : MenuAbstract("Change pattern") {
		hasSelection = true;
	}

	String getData() {
		return String(Data::currentPattern);// .c_str();
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
		patterns.inc();
	}
	void dec() {
		patterns.dec();
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
				r += "/t";
				r += getItemData(i);
				r += "/n";
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

	static const size_t numItems = 2;
	MenuAbstract *items[numItems] = {
		&CurrentPattern,
		&PatternList,
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


	//menu items
	MenuCurrentPalette CurrentPalette;

	static const size_t numItems = 1;
	MenuAbstract *items[numItems] = {
		&CurrentPalette
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
	PageSettings() : MenuPage("Settings") {}

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


	//menu items
	static const size_t numItems = 1;
	MenuAbstract *items[numItems] = {
		&baseMenu
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
		if (i >= numPages)
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
			r += "/t";
			r += pages[i]->getItemData(j);
			r += "/n";

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
		return r;
	}
};

Menu menu;
