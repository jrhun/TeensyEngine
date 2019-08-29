#pragma once

#include <vector>

// Takes a reference to a value and implements an interface for changing it within defined min and max + deltas
// Also provides a way to oscilate and change that value

class Oscilate {
public:
	Oscilate() {}


};
/*
template<class TYPE>
class VariableControl {
public:
	VariableControl() {}
	VariableControl(TYPE &v, char* name, TYPE min, TYPE max, TYPE delta) :
		var(v), name(name), min(min), max(max), 
		delta(delta), diff(max-min) {
		//variables.push_back(this);
	}


	static std::vector<VariableControl*> variables;


	//bool addVar(TYPE v, char* name) {
	//	return false;
	//}

	char* getName() {
		return name;
	}

	TYPE getVarVal() {
		return var;
	}

	TYPE& getVarRef() {
		return var;
	}

	void inc() {
		if (var + delta > max) {
			var += delta;
		}
		else
			var = max;
	}

	void dec() {
		if (var - delta < min) {
			var -= delta;
		}
		else
			var = min;
	}

	void trigger() {
		var = max;
		stepVal = 0;
		decayExp();
	}

	static void triggerAll() {
		for (auto v : variables) {
			v.trigger();
		}
	}

	static void updateAll() {
		for (auto v : variables) {
			v.update();
		}
	}

	void update() {
		if (triggerActive) {
			float step = 1.0 / (diff); // goes from 0 to 1
			val = max * pow(float(min / max), stepVal);
			stepVal += step;
			if (val = min)
				triggerActive = false;
		}
	}

private:

	TYPE &var;
	TYPE min;
	TYPE max;
	TYPE diff;
	TYPE delta;
	float stepVal;
	bool triggerActive = true;
	char* name;
};

template<>
void VariableControl<bool>::trigger() {
	var = !var;
}

template<>
void VariableControl<bool>::update() {}

*/

//class PatternVariable;
//
//template<class TYPE>
//class TriggerSystem {
//public:
//	TriggerSystem(TYPE &v) : var(v) {}
//
//	void trigger() {
//
//	}
//
//
//	TYPE &v;
//};