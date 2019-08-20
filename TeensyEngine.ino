#include "data.h"
#include "ledControl.h"
#include "src\Engine.h"


GraphicsFastLED gfx_t(&ledControl);
Engine engine(&gfx_t);

void setup() {
  // put your setup code here, to run once:
//  engine.init();
  ledControl.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  EVERY_N_MILLISECONDS(15) {
    ledControl.clear();
    engine.drawTest();
    engine.gfx->putPixel(5,10, CRGB::Blue);
    ledControl.show();
  }
}
