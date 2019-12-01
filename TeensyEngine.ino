#include "data.h"
#include "ledControl.h"
#include "audio.h"
#include "motion.h"

#include "src\Patterns\PatternController.h"
#include "ui.h"

//#include "headbands.h"



void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Started");
  
//  audioSetup();
  ledControl.init();
  uiSetup();

  initMPU();
}

void loop() {
//  audioLoop();

  EVERY_N_MILLISECONDS(50) {
    getPitchBasic();
  }

//  if (Data::audioOn) audioLoop();
//  if (Data::MPUOn) updateMPU();
  patterns.run();
  uiLoop();

  

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
