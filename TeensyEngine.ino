#include "data.h"
#include "ledControl.h"

#include "src\Patterns\PatternController.h"
#include "ui.h"



//GraphicsFastLED gfx_t(&ledControl);
//Engine engine(gfx_t);



void setup() {
  // put your setup code here, to run once:
//  engine.init();
  ledControl.init();
  Serial.begin(115200);

  uiSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  patterns.run();
  uiLoop();
  
//  EVERY_N_MILLISECONDS(15) {
//    engine.clear();
//    engine.updateModel();
//    engine.composeFrame();
//    engine.show();
//  }
  EVERY_N_MILLISECONDS(1000) {
    Serial.print("FPS: ");
    Serial.println(FastLED.getFPS());
    tftDisplayFPS();
  }
}
