#pragma once


#include "data.h"
#include "src\Patterns\PatternController.h"

#include <SPI.h>
#include <RF24.h>


//#include "audio.h"
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);

const byte serverAddress[6] = "0Core";      // all clients send to 0Core, server listens on 0Core
const byte multicastAddress[6] = "1Core";   // all clients listen on 1Core
const uint8_t numClients = 12;
byte addressTable[numClients][6] = {"0Node", /*"1Node",*/ "2Node", "3Node", "4Node",
                                    "5Node", "6Node", "7Node", "8Node", 
                                    "9Node", "aNode", "bNode", "cNode"
                                   };
uint8_t radioBuffer[32];
uint8_t retries = 5;
enum UpdateType {
  TRIGGER = 1, UPDATE = 4,
  SETTINGS = 5, PALETTE = 21, PALETTE2 = 27
};
enum {manualBit = 0, fadeBit = 1};

void setupRadio() {
  radio.begin();

  //  radio.setChannel(1);
  radio.setAutoAck(1);
  radio.enableDynamicPayloads();
  radio.enableAckPayload();
  radio.enableDynamicAck(); //not working on my radios, no ack sent at all!
  //  radio.setCRCLength(RF24_CRC_8);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);

  radio.setRetries(5, retries);
}

uint8_t prepareUpdate(UpdateType t) {
  //returns size of update
  CRGBPalette16 tempPalette;
  switch (t) {
    case TRIGGER: // payloadSize = 1
      radioBuffer[0] = Data::hue;
      break;
    case UPDATE: // payloadSize = 4
      radioBuffer[0] = Data::getHue();
//      radioBuffer[0] = avgLevel / 32;
      radioBuffer[1] = Data::hbHueOffset;
      radioBuffer[2] = Data::hbIndex;
      radioBuffer[3] = Data::hbSpeed;
      break;
    case SETTINGS: // payloadSize = 5
      radioBuffer[0] = Data::hbOn ? Data::hbBrightness : 0; //brightness
      radioBuffer[1] = uint8_t ((Data::hbFadeAmount / 255.0) * Data::hbBrightness); // fade amount (max fade is current brightness)
      radioBuffer[2] = uint8_t( (0b0011 & (Data::hbManual << manualBit | Data::hbFade << fadeBit) ) );// | uint8_t(Data::hbHueDelay << 2);
      radioBuffer[3] = uint8_t(Data::hbHueDelay);
      radioBuffer[4] = Data::hbCurrentPattern;
      break;
    case PALETTE: // payloadSize = 21
      // send first 21bits of palette/first 7 CRGB entries
      if (Data::syncPalettes) 
        memcpy(radioBuffer, gfx.currentPalette.entries[0].raw, 21);
      else {
        tempPalette = gGradientPalettes[Data::hbPaletteIndex];
        memcpy(radioBuffer, tempPalette.entries[0].raw, 21);
      }
      break;
    case PALETTE2: // payloadSize = 27
      // send last 27bits of palette/last 9 CRGB entries
      if (Data::syncPalettes) 
        memcpy(radioBuffer, gfx.currentPalette.entries[7].raw, 27);
      else {
        tempPalette = gGradientPalettes[Data::hbPaletteIndex];
        memcpy(radioBuffer, tempPalette.entries[7].raw, 27);
      }
      break;
  }
  return (uint8_t)t;
}

void sendClientUpdate(UpdateType t, uint16_t clientIDmask = 1023) {
  if (t == UPDATE || t == TRIGGER) {
    radio.openWritingPipe(multicastAddress);
//    radio.setAutoAck(0, 0);
    uint8_t payloadSize = prepareUpdate(t);
    radio.write(&radioBuffer, payloadSize, true);
    radio.write(&radioBuffer, payloadSize, true);
    radio.write(&radioBuffer, payloadSize, true);
    radio.write(&radioBuffer, payloadSize, true);
    radio.write(&radioBuffer, payloadSize, true);
//    radio.setAutoAck(0, 1);
  }
  else {
    for (uint8_t i = 0; i < numClients; i++) {
      if (bit_is_set(clientIDmask, i)) {
        radio.openWritingPipe(addressTable[i]);
        uint8_t payloadSize = prepareUpdate(t);
        if (radio.write(&radioBuffer, payloadSize)) {
          //          printf("Sent update %i to %s\n", t, addressTable[i]);
          if (t == PALETTE) {
            payloadSize = prepareUpdate(PALETTE2);
            if (radio.write(&radioBuffer, payloadSize)) {
//                printf("Sent Palette1+2 to %s\n", addressTable[i]);
            }
          }
        }// end write
      }
    } // end loop
  } // end if
}



void updateHeadbandsPalette() {
  
}
