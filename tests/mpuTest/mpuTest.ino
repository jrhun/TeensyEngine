/*
Basic_I2C.ino
Brian R Taylor
brian.taylor@bolderflight.com

Copyright (c) 2017 Bolder Flight Systems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MPU9250.h"


// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire2,0x68);
int status;


#include "SPI.h"
//#include "ILI9341_t3.h"
//#include "font_Arial.h"

//#include "SPI.h"
#include "ILI9341_t3n.h"
#include "ili9341_t3n_font_Arial.h"
#include "ili9341_t3n_font_ArialBold.h"
#define TFT_DC      32
#define TFT_CS      30
#define TFT_RST     31
#define TFT_BACKLIGHT 33
#define TFT_MOSI    11
#define TFT_SCLK    13
#define TFT_MISO    12

ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST);

#define TFT_BACKGROUND  0x2967
#define TFT_TEXT        0xE719


void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  // setting the accelerometer full scale range to +/-8G
  //  aRes = 8.0/32768.0;
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting the gyroscope full scale range to +/-500 deg/s
  //  gRes = 500.0/32768.0;
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU.setSrd(19);


  //tft
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, LOW);

  tft.begin();
  //  tft.setFont(Arial_24_Bold);
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLACK);// grey fill
  analogWrite( TFT_BACKLIGHT, 128);
}

void loop() {
  // read the sensor
  IMU.readSensor();
  // display the data
  Serial.print(IMU.getAccelX_mss(),6);
  Serial.print("\t");
  Serial.print(IMU.getAccelY_mss(),6);
  Serial.print("\t");
  Serial.print(IMU.getAccelZ_mss(),6);
  Serial.println("\t");
  char t[20];
  tft.useFrameBuffer(true);
  tft.fillScreen(ILI9341_BLACK);// grey fill
  tft.setTextSize(2);
  tft.setTextColor(TFT_TEXT);
  tft.setCursor(0, 0);
  sprintf(t, "aX: %.6f", IMU.getAccelX_mss());
  tft.println(t);
  sprintf(t, "aY: %.6f", IMU.getAccelY_mss());
  tft.println(t);
  sprintf(t, "aZ: %.6f", IMU.getAccelZ_mss());
  tft.println(t);
  tft.updateScreenAsync(false);
  delay(100);
}
