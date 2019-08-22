#include "data.h"
#include "ledControl.h"
#include "srch\Engine.h"


GraphicsFastLED gfx_t(&ledControl);
Engine engine(gfx_t);

void setup() {
  // put your setup code here, to run once:
//  engine.init();
  ledControl.init();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  EVERY_N_MILLISECONDS(15) {
    ledControl.clear();
    engine.updateModel();
    engine.composeFrame();
    ledControl.show();
  }
  EVERY_N_MILLISECONDS(1000) {
    Serial.print("FPS: ");
    Serial.println(FastLED.getFPS());
//    tftDisplayFPS();
  }
}
