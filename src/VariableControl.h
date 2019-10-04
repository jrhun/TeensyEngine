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


// use variable reference for UI functions
// control a variable between min and max values
template<class TYPE>
class VariableReference {
public:
	VariableReference() : min(0), max(0), wrap(true) {}
	VariableReference(const char* name, TYPE &t, TYPE min = 0, TYPE max = 255, bool wrap = false) : name(name), d(t), min(min), max(max), wrap(wrap) {}

	const char* name;
	String getName() {
		return name;
	}

	String getValue() {
		return to_string(d);
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

};

typedef VariableReference<float>		FloatReference;
typedef VariableReference<int>			IntReference;
typedef VariableReference<unsigned int> uIntReference;
typedef VariableReference<int8_t>		Int8Reference;
typedef VariableReference<uint8_t>		uInt8Reference;



class VariableOscilate {
public:
	typedef uint8_t type;
	VariableOscilate() : var(0), min(0), max(255) {}
	VariableOscilate(type min, type max) : var(0), min(min), max(max) {

	}

	enum { RAMP = 0, INVERSE_RAMP, TRIANGLE, SQUARE, SIN, TRIGGER, OFF };




	void trigger(uint16_t d = 200) {
		triggerActive = true;
		triggerStart = GET_MILLIS();
		triggerDuration = 200;
		oscType = TRIGGER;
		var = max;
	}

	void setType(uint8_t i) {
		if (i <= OFF) {
			oscType = i;
		}
	}

	void setBPM(accum88 b) {
		bpm = b;
	}
	accum88 getBPM() {
		return bpm;
	}

	void update() {

		smoothTimebase = 0.8 * smoothTimebase + 0.2 * timebase;

		unsigned long now = GET_MILLIS() - timebase;

		accum88 b = bpm;
		if (halfBPM)	b /= 2;
		if (quaterBPM)	b /= 4;


		switch (oscType) {
		case RAMP:
			/* Ramp	/|/|/|			*/
			var = (((now * (b) * 280) >> 16) >> 8);
			break;
		case INVERSE_RAMP:
			/* Inverse ramp |\|\|\	*/
			var = 255 - (((now * (b) * 280) >> 16) >> 8);
			break;
		case TRIANGLE:
			var = (((now * (b) * 280) >> 16) >> 8);
			if (var > 128)
				var /= 2;
			else if (var <= 128)
				var = 256 - (var * 2);
			break;
		case SQUARE:
			var = (((now * (b) * 280) >> 16) >> 8);
			var += 128;
			if (var > 128)
				var = 255;
			else var = 0;
			break;
		case SIN:
			var = sin8(((now * (b) * 280) >> 16) >> 8);
			break;
		case TRIGGER:
			// Trigger decay
			if (triggerActive and var > min) {
				var = myMap(now - triggerStart, 0, triggerDuration, max, min, true);
			}
			else { triggerActive = false; var = min; }
			break;
		case OFF:
		default:
			break;
		}
	}


	//static std::vector<VariableOscilate*> activeOscilaters;

	type& operator* () {
		return var;
	}

	bool halfBPM = false;
	bool quaterBPM = false;

	void sync() {
		if (oscType == TRIGGER) {
			trigger();
		}
		else {
			unsigned long now = GET_MILLIS();
			
			timebase = now;

			uint16_t tpt = 0; // average time per tap

			if (prevBeat) {
				if (now - prevBeat > 4000) {// four second timeout, reset count and sum
					nBeats = 0;
					sum = 0;
				}
				else {
					nBeats++;
					sum += 15360000L / (now - prevBeat); // 60,000 / 500ms = 120 BPM
					tpt = sum / nBeats; //BPM * 256
					bpm = tpt;
				}
			}
			prevBeat = now;
		}
	}

private:
	type var;
	type min;
	type max;

	uint16_t triggerDuration = 200;
	unsigned long triggerStart = 0;
	bool triggerActive = false;

	accum88 bpm = (120 << 8);
	uint8_t oscType = OFF;

	unsigned long timebase = 0;
	unsigned long smoothTimebase = 0;

	// from https://learn.adafruit.com/tap-tempo-trinket/code
	unsigned long
		prevBeat = 0L, // Time of last button tap
		sum = 0L; // Cumulative time of all beats
	uint16_t
		nBeats = 0;  // Number of beats counted



};


template<class T>
class VariableControl {
public:
	VariableControl() {}

	VariableControl(T &var) : var(var), min(0), max(255) {
		delta = max - min;
		triggerEnd = min;
	}

	VariableControl(T &var, T _min, T _max) : var(var), min(_min), max(_max) {
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
				//static bool dirUp = true;
				//bool w = var.wrap;
				//var.wrap = false;
				//if (dirUp) {
				//	var.inc();
				//	if (var.d >= max)
				//		dirUp = false;
				//}
				//else {
				//	var.dec();
				//	if (var.d <= min)
				//		dirUp = true;
				//}

				//var.wrap = w;
			}

			else if (rampOn) {
				//bool w = var.wrap;
				//var.wrap = true;
				//var.inc();
				//var.wrap = w;
			}

			else if (triggerOn) {
				//bool w = var.wrap;
				//var.wrap = false;
				//var.dec();
				//if (var.d <= triggerEnd) {
				//	triggerOn = false;
				//	var.d = triggerEnd;
				//}
				//var.wrap = w;
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
	VariableReference<T> &var;
	T min;
	T max;
	T delta;
	T triggerEnd;

	bool oscilateOn = false;
	bool rampOn = false;
	bool triggerOn = false;
	float interval = 10;
};