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
#define JOY_UP_PIN      4
#define JOY_LEFT_PIN    6
#define JOY_DOWN_PIN    2
#define JOY_RIGHT_PIN   3

#define EQ_RESET_PIN    9
#define EQ_STROBE_PIN   10
#define EQ_AUDIO_PIN    26  //A12
#define MIC_IN_PIN      27  //A13
#define AMP_IN_PIN      21  //A7
#define VOLTS_IN_PIN    20  //A6

#define OLED_DC         32
#define OLED_RS         31
#define OLED_CS         30 
#define OLED_LED_PIN    33

#define LED1            19
#define LED2            18
#define LED3            14
#define LED4            15
#define LED5            17
#define LED6            16
#define LED7            22
#define LED8            23


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

namespace Data {
	//  public:
	// VARS

	// Led control variables
	uint8_t brightness = 24;
	uint8_t fadeAmount = 48;
	bool adjustGamma = false;

	uint8_t hueScale = 3;
	uint8_t hueInc = 1;
	uint16_t hue = 0;
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
	uint8_t currentPalette = 0;

	// audio vars
	uint8_t noiseFloor = 65;


} //end nameSpace Data

float GuiVars1 = 1.6; //1.6, 0.6 1.25, 0.87, 1.25, 1.15
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
},
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
