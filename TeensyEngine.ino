#include "data.h"
#include "ledControl.h"

#include "src\Patterns\PatternController.h"
#include "ui.h"


void setup() {
  // put your setup code here, to run once:
  ledControl.init();
  Serial.begin(115200);

  uiSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  patterns.run();
  uiLoop();
  
  EVERY_N_MILLISECONDS(1000) {
    Serial.print("FPS: ");
    Serial.println(FastLED.getFPS());
    tftDisplayFPS();
  }
}
