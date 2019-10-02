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


	//static std::vector<VariableControl*> variables;


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
	VariableControl();

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



	void oscilate(accum88 BPM) {
		float period = (60000.0f / (BPM / 256.0f)); // e.g. BPM = 100, delta is 100, period = 600 
		interval = period / (delta * 2.0f);  // then interval = 3
		oscilateOn = true;
		rampOn = false;
		triggerOn = false;
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
	void trigger(unsigned long length = 200, T _max = 0) {
		if (_max == 0) {
			_max = max;
		}

		interval = round(float(length) / float(delta)); // = 2 if length = 200 and max - min = 100

		triggerEnd = var.d;
		var.d = _max;

		triggerOn = true;
		rampOn = false;
		oscilateOn = false;
	}

private:
	VariableContainer<T> &var;
	T min;
	T max;
	T delta;
	T triggerEnd;

	bool oscilateOn = false;
	bool rampOn = false;
	bool triggerOn = false;
	float interval = 10;
};