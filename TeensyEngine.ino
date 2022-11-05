#include "data.h"
//#include "ledControl.h"
#include "audio.h"
#include "motion.h"

#include "src\Patterns\PatternController.h"
#include "ui.h"

//#include "headbands.h"



void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Started");
  
  audioSetup();
  
//  pinMode(MIC_IN_PIN, INPUT);
//  pinMode(EQ_AUDIO_PIN, INPUT);

  gfx.init();
//  ledControl.init();
  uiSetup();

  initMPU();

  //ADC setup
  //A6 volt, A7 amp on ADC0, A13 audio on ADC1


  
}

void loop() {
  audioLoop();
  
  EVERY_N_MILLISECONDS(50) {
    if (Data::accelerometer)
      getPitchBasic();
    else {
      Data::pitch = beatsin8(30, 0, 60) - 30;
      Data::roll = beatsin8(32, 0, 60) - 30;
    }
  }

//  if (Data::audioOn) audioLoop();
//  if (Data::MPUOn) updateMPU();
  patterns.run();
  uiLoop();
  
//  EVERY_N_SECONDS(Data::changePaletteDelay ? Data::changePaletteDelay : 1) {
//  	thistimer.setPeriod
//  }

  

//  if (Data::sendUpdate) {
//    hbUpdate(Data::sendUpdate);
//    Data::sendUpdate = 0;
//  }
//  
//  EVERY_N_MILLISECONDS(1000) {
//    Serial.print("FPS: ");
//    Serial.println(FastLED.getFPS());
////    tftDisplayFPS();
//  }
}
