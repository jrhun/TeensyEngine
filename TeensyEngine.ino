#include "data.h"
#include "ledControl.h"
#include "src\Engine.h"


//#include "src\Patterns\Pattern.h"

GraphicsFastLED gfx_t(&ledControl);
Engine engine(gfx_t);
//PatternController patterns;



void setup() {
  // put your setup code here, to run once:
//  engine.init();
  ledControl.init();
  Serial.begin(115200);
  Pattern::setGfx(&gfx_t);
}

void loop() {
  // put your main code here, to run repeatedly:
  EVERY_N_MILLISECONDS(15) {
    engine.clear();
    engine.updateModel();
    engine.composeFrame();
    engine.show();
  }
  EVERY_N_MILLISECONDS(1000) {
    Serial.print("FPS: ");
    Serial.println(FastLED.getFPS());
//    tftDisplayFPS();
  }
}
