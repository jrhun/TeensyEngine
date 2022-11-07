#ifndef DATA_H
#define DATA_H


// PIN DEFINITIONS
#if defined(ESP32)
#define EN_A_PIN        34
#define EN_B_PIN        35
#define JOY_CENTRE_PIN  22
#define JOY_UP_PIN      15
#define JOY_LEFT_PIN    2
#define JOY_DOWN_PIN    4
#define JOY_RIGHT_PIN   3

#define EQ_RESET_PIN    19
#define EQ_STROBE_PIN   21
#define OLED_LED_PIN    1
#define EQ_AUDIO_PIN    36  // ADC1 CH0
#define AMP_IN_PIN      39

#define OLED_DC 16
#define OLED_RS 17
#define OLED_CS 5

#endif // esp32 defines
// teensy 4.x
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) 
#define EN_A_PIN        1
#define EN_B_PIN        0
#define JOY_CENTRE_PIN  5
#define JOY_UP_PIN      2
#define JOY_LEFT_PIN    6
#define JOY_DOWN_PIN    4
#define JOY_RIGHT_PIN   3

#define EQ_RESET_PIN    9
#define EQ_STROBE_PIN   10
#define EQ_AUDIO_PIN    26  //A12
#define MIC_IN_PIN      27  //A13
#define AMP_IN_PIN      21  //A7
#define VOLTS_IN_PIN    20  //A6

#define NRF_CE_PIN      29
#define NRF_CSN_PIN     28
#define ESP_RX          28 //RX7
#define ESP_TX          29 //TX7

// Normal Connections
#define TFT_DC      32
#define TFT_CS      30
#define TFT_RST     31
#define TFT_BACKLIGHT 33

#define LED1            19
#define LED2            18
#define LED3            14
#define LED4            15
#define LED5            17
#define LED6            16
#define LED7            22
#define LED8            23


// MPU9250 on SCL2/ SDA2

#include <ADC.h>
ADC *adc = new ADC(); 

#endif // teensy 4.x

#define NUM_VARS    6
#define NUM_BUTTONS 6

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH	64
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT	32
#endif

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#if defined(ARDUINO)
//typedef String String

#else
#include <string>
typedef std::string String;
#define PROGMEM
#endif

// use variable reference for UI functions
// control a variable between min and max values
//template<class TYPE>
class VariableReference {
	typedef uint8_t TYPE;
public:
	VariableReference() : d(NULL), starting(128), min(0), max(0), wrap(true) {}
	VariableReference(const char* name, TYPE *t, TYPE starting = 128, TYPE min = 0, TYPE max = 255, bool wrap = false) : name(name), d(t), starting(starting), min(min), max(max), wrap(wrap) {}

	const char* name;
	String getName() {
		return name;
	}

	String getValue() {
#if defined(ESP32) || defined(CORE_TEENSY)
		return String(*d);
#else
		return to_string(*d);
#endif
	}
	TYPE *d;
	TYPE starting;
	TYPE min;
	TYPE max;
	void(*callback)() = NULL;
	bool wrap;

	void setCallback(void(*fn)()) { callback = fn; }

	void inc(TYPE amount = 1) {
		if (*d + amount > max) {
			if (wrap) 	*d = *d - max + amount;
			else 		*d = max;
		} else {
      *d += amount;
		}
		if (callback) {
			callback();
		}
	}

	void dec(TYPE amount = 1) {
		if (*d - amount < min) {
			if (wrap) 	*d = *d - amount + max; // works for both signed/unsigned + floats
			else		*d = min;
		}
		else			*d -= amount;
		if (callback) {
			callback();
		}
	}

	TYPE& operator * (void) {
		return *d;
	}

// TYPE& operator & (void) {
//   return *d;
//  }

};

namespace Data {
	//  public:
	// VARS

	// Led control variables
	uint8_t brightness = 24;
	uint8_t fadeAmount = 48;
	bool adjustGamma = false;

	VariableReference brightness_t("Brightness", &brightness, 24);

	uint8_t hueScale = 3;
	uint8_t hueInc = 1;
	uint16_t hue = 0;
	bool hueChange = true;
	uint8_t hueSpeed = 30;
	uint8_t getHue() {
		return hue / hueScale;
	}
	void incHue() {
		hue = (hue + hueInc) % (256 * hueScale);
	}

	//pattern vars
	uint8_t FPS = 90;

	// UI variables
	uint8_t currentPattern = 0;

 
	uint8_t paletteIndex = 0;
  uint8_t hbPaletteIndex = 0;
  uint8_t changePaletteDelay = 0; //seconds

  uint32_t colorTemp = 0;




	// audio vars
  bool audioOn = false;
	uint8_t noiseFloor = 65;
  uint8_t audioThreshold = 128;
  float beat = 0;

  uint8_t maxVol = 40;

  const uint8_t numSamples = 16;
  uint16_t sampleArray[numSamples];
  uint16_t sampleSum = 0;
  uint16_t sampleAvg = 0;
  uint16_t sampleCount = 0;
  uint8_t samplePeak = 0;
  float samplePeakVal = 0.0f;
  unsigned long peakTime;


  // backlight
  uint8_t backlight_brightness = 63;
  VariableReference backlight_t{"Backlight", &backlight_brightness, 63, 0, 63};
  
  uint8_t triggerType = 0;
  VariableReference triggerType_t{"Trigger", &triggerType, 4, 0, 6};

  
  //text
  bool textOn = false;
  uint8_t scrollSpeed = 1;
  uint8_t textIndex = 0;
  const uint8_t textOptionsCount = 17; // custom options at index 15 and 16
  const uint8_t customIndex1 = textOptionsCount-2;
  const uint8_t customIndex2 = textOptionsCount-1;
  VariableReference textIndex_t{"Text", &textIndex, 0, 0, textOptionsCount};
  const String textOptions[textOptionsCount] = {
    "Dance!",
    "Golden!",
    "Good job!",
    "Well done!",
    "Good tune!", 
    "Tune!",
    "Nice one!", 
    "Thanks!",
    "Yew!",
    "Wow!", 
    "Boot!",
    "Love it!",
    "Kick it!", 
    "Hi Mum!",
    "Nailed it!",
    "Custom 1",
    "Custom 2"
  };

  char custom1[12] = "           ";
  char custom2[12] = "           ";

  //accelerometer
  bool MPUOn = false;
//  float ax = 0.0f, ay = 0.0f, az = 0.0f;
//  float gx = 0.0f, gy = 0.0f, gz = 0.0f;
  float pitch = 30.0f;
  float roll = 0.0f;

  //headbands
  enum {hbPaletteUpdate = 1, hbBrightnessUpdate, hbPatternUpdate}; // update type
  uint8_t sendUpdate = 0;

  bool hbOn = true;
  bool hbManual = false;
  bool hbFade = false;
  bool hbSendUpdate = true;
  bool syncHue = true;//not used
  uint16_t hbSendMillisDelay = 100;
  uint8_t indexSkip = 1;
  uint8_t hbHueOffset = 50;
  uint8_t hbHueDelay = 16;
  uint8_t hbIndex = 0;
  uint8_t hbSpeed = 50;
  uint8_t hbBrightness = 128;
  uint8_t hbFadeAmount = 32;
  uint8_t hbCurrentPattern = 2;
  const uint8_t hbMaxPatterns = 6;
  const String hbPatternNames[hbMaxPatterns] = { 
    "Wipe Colours", 
    "Rotate Colours", 
    "Rotate Colours2", 
    "Sparkles", 
    "Commets", 
    "Solid colour",
  };

  VariableReference brightnessHb_t("Brightness", &hbBrightness, hbBrightness, 0, 255);
  VariableReference fadeHb_t("Fade", &hbFadeAmount, hbFadeAmount, 0, 128);
  VariableReference speedHb_t("Speed", &hbSpeed, hbSpeed, 0, 255);
  VariableReference hueDelayHb_t("Hue delay", &hbHueDelay, hbHueDelay, 1, 64);

//  VariableReference hbPattern_t{"HB Pattern", &hbCurrentPattern, 2, 0, hbMaxPatterns};

  bool syncPalettes = false;

  bool dither = false;
  bool accelerometer = true;


} //end nameSpace Data

float GuiVars1 = 1.0; //1.6, 0.6 1.25, 0.87, 1.25, 1.15
float GuiVars2 = 0.6;
float GuiVars3 = 1.25;
float GuiVars4 = 0.87;
float GuiVars5 = 1.5;
float GuiVars6 = 1.2;

uint8_t FxFade = 128;
uint8_t FxBlur = 0;
uint8_t FxSpiral = 0;
uint8_t FxNoiseSmear = 0;


uint8_t vars[NUM_VARS] = {
  7, 25, 4, 1, 1, 0
};

//uint8_t *speed = &vars[0];
//uint8_t *scale = &vars[1];

String varsName[NUM_VARS] = {
  "Speed", "Scale", "palette", "var4", "var5", "var6"
};


bool buttons[NUM_BUTTONS] = { 0, 1, 1, 1, 0, 0 }; // gamma, dither, on










static const uint8_t PROGMEM
gamma5[] = {
  0x00, 0x01, 0x02, 0x03, 0x05, 0x07, 0x09, 0x0b,
  0x0e, 0x11, 0x14, 0x18, 0x1d, 0x22, 0x28, 0x2e,
  0x36, 0x3d, 0x46, 0x4f, 0x59, 0x64, 0x6f, 0x7c,
  0x89, 0x97, 0xa6, 0xb6, 0xc7, 0xd9, 0xeb, 0xff
};
static const uint8_t PROGMEM
gamma6[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08,
  0x09, 0x0a, 0x0b, 0x0d, 0x0e, 0x10, 0x12, 0x13,
  0x15, 0x17, 0x19, 0x1b, 0x1d, 0x20, 0x22, 0x25,
  0x27, 0x2a, 0x2d, 0x30, 0x33, 0x37, 0x3a, 0x3e,
  0x41, 0x45, 0x49, 0x4d, 0x52, 0x56, 0x5b, 0x5f,
  0x64, 0x69, 0x6e, 0x74, 0x79, 0x7f, 0x85, 0x8b,
  0x91, 0x97, 0x9d, 0xa4, 0xab, 0xb2, 0xb9, 0xc0,
  0xc7, 0xcf, 0xd6, 0xde, 0xe6, 0xee, 0xf7, 0xff
};

static const uint8_t PROGMEM
decayData[] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,
	1,1,1,1,1,1,2,2,
	2,2,2,3,3,4,4,5,
	5,6,7,8,9,10,11,13,
	14,16,18,21,24,27,30,35,
	39,44,50,57,64,73,83,94,
	106,120,136,155,175,199,225,255
};

uint8_t decayExp(uint8_t i) {
	uint8_t d = i / 4;
	uint8_t r = i % 4; //0 1 2 3
	if (d == 63)
		return decayData[d];// *r + decayData[d - 1] * (3 - r);
	return (decayData[d] * (3 - r) + decayData[d + 1] * r) / 3;
}




#endif //DATA_H
