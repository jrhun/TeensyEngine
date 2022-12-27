# TeensyEngine

TeensyEngine is primarily the software I use for my Totem Pole - named *The Orb*. 

Included in this repository is a description of both the hardware and software features, including my MCAD/ECAD designs for everything.
It includes an emulator that's using OpenFrameworks for display and UI to allow easy testing and development.


## Contents 
- [About](#about)
- [Features](#features)
- [Hardware](#hardware)
- [Software](#software)
- [Credit](#credit)


## About
I took ~2000 LED lights and carefully shoved them inside a polycarbonate ball, then hooked them all up to a Teensy. Shockingly, it worked quite well. 
This has been a multiyear endevour that I've slowly added more and more features to.
It's the 4th iteration of the hardware including LEDs and controller/batery box. The first version was a couple of LEDs taped down inside two salad bowls - how far this project has come!

Just a heads up - I'm a completely self taught programmer/3d modeler/electronics designer, there is a level of *jank* at times that should be expected to come of that but overall I've been very pleased with the outcome! 
I will glady take any suggestions or tips to improve my work, please comment any suggestions!


## Features
- 25+ different patterns with the ability to change the colour pallete being used for most patterns!
- Custom global blur/fade/glitter effects that can run on top of any pattern
- Text overlay onto patterns with the ability to enter custom text using the controller
- An accelerometer - because why not
- LCD Screen for changing settings/patterns + showing battery voltage and power usage
- Encoder + buttons for navigating the UI
- Microphone input 
- Wireless compatibility with NRF24L01 or ESP-01
- Current sensor for monitoring power usage


## Hardware
#### Processor
A teensy 4.0 is the heart of the project, providing plenty of raw power to overcome my inefficient coding and push out LEDs while controling a UI. 

#### Accessories
- MPU9250 accelerometer
- Microphone - I can't remember what I'm using, one of the sparkfun electret autogain boards. 
- NRF24l01 - used to connect wirelessly to LED headbands - allows you to change the patterns/brightness/colour of the headbands!
- ESP-01 - pins to connect to an ESP are broken out but not yet implemented
- ACS712 current sensor in series with the LEDs to measure power usage
- MSGEQ7 - the pinout is there but I haven't ended up using it yet

#### Lights
Custom 3d printed scaffolding for strips of SK6805-2427 RGB LEDs, to hold them in a sphereical shape inside a polycarbonate ball 25cm in diameter. They're held slightly back from the inside of the sphere to help diffuse the lights nicely. 
The LED strips are arranged with an offset pattern to attempt to provide more resolution in the centre and less at the poles, allowing for an effective resolution of 32H x 64W. 

The lights are from [Aliexpress](https://www.aliexpress.com/item/32818340106.html?spm=a2g0o.order_list.order_list_main.358.679f1802EQyb32), and were chosen for their high density (120/M, 8.33mm pitch) and thin strips (6mm width). 
Lights with a smaller pitch and thinner stip were outrageously expensive at the time I was purchasing LEDs, but have since come down in price somewhat... Version 5 [TBC](https://www.aliexpress.com/item/1005003798198621.html?spm=a2g0o.productlist.main.1.1ec91845yMUVny&algo_pvid=75d3a123-2f4a-48b9-b82f-fbbad28e56ed&algo_exp_id=75d3a123-2f4a-48b9-b82f-fbbad28e56ed-0&pdp_ext_f=%7B%22sku_id%22%3A%2212000028080815414%22%7D&pdp_npi=2%40dis%21AUD%2140.15%2126.11%21%21%21%21%21%40211bf14716719782508633629d0742%2112000028080815414%21sea&curPageLogUid=63plojRdJNp2)?

#### Power
The whole thing is powered by 4x 18650 batteries, I usually carry a spare set of 4x batteries for each night it's used which seems to last the night through.
There's a regulator in the controller providing 5V for the teensy and accessories. The full battery voltage is sent up to the LEDs where there is a seperate Pololu [D24V150F5](https://www.pololu.com/product/2881) Step-down convertor. This thing is expensive (and seems to have almost doubled in price since I bought one - ouch) but can handle up to 15A at 5V reliably and pretty efficiently, which is important since the LEDS draw ~0.5A even when off with just their standby current! (2.5microamps adds up when there are almost 2000 leds...)

The benefit of the two power supplies is efficiency and stability - it avoids the much higher current that would be needed from sending 5V to the LEDs (and the voltage drop since it's a decent 1M run!), and a seperate and stable power source for the controller that doesn't dip when the LEDs draw a bunch of power. 


## Software
The software include a basic emulator that runs the same UI and pattern software using Openframeworks to display the LEDs. 

I've tried to organise the UI controls into roughly similar pages.
#### Pattern controls
- Select desired patterns from the list of patterns
- Change overall brightness
- Select a tempo + tap tempo
- Change the tempo oscillator for adding differing motion to patterns
- Trigger ability to change a patterns settings (e.g. scroll through different settings for a pattern without making a complete new one)
#### Colour controls
- Change current pallete with a palette preview option
- Blend amount when changing palletes over - quick or slow blend
- Change how fast colours from a palette are cycled through
#### Text/Fx controls
- Add transparent or opaque text over any pattern, or just blink text 
- Text scroll speed
- Select the text from a list or enter custom text (12 char max only!)
- Global blur/fade/glitter effects
#### Wireless controlls
- Control the wireless headbands I made using NRF24L01 communication
- Change the pattern/colour/brightness of headbands in range
#### General settings
- LCD Backlight/Dither/Colour temp/Accelerometer/Save/load settings

## Arduino code
The Teensyduino core was used for compiling and uploading.

Arduino libraries used:
- [ILI9341_t3n](https://github.com/KurtE/ILI9341_t3n)
- [Encoder](https://github.com/PaulStoffregen/Encoder/blob/master/Encoder.h) from Paul Stroffregen
- [MPU9250](https://github.com/kriswiner/MPU9250) from kriswiner
- [FastLED](https://github.com/FastLED/FastLED)
- [Teensy ADC](https://github.com/pedvide/ADC)
- [RF24](https://github.com/nRF24/RF24)


## PC Emulator 
#### Requirements 
Openframeworks 0.10.1 was used for the graphics, with the ofxGui addon.

Visual Studio 2019 was used for compiling.
At one point I had it cross compiling on mac, but I haven't checked if that still works in awhile.

The emulator has keyboard controls for the UI:
- WASD: joystick up/left/down/right
- Q/E: encoder rotate clockwise/counterclockwise
- F: joystick press 
- space: seperate trigger, triggers effect on certain patterns (calls Pattern::Trigger())
- Home/End/Del/PgDown: change pitch/roll level to emulate accelerometer changes
- I: add random impulse to accelerometer data
- 1 through 7: change beat type (RAMP, INVERSE_RAMP, TRIANGLE, SQUARE, SIN, TRIGGER, GATE, AUDIO, AUDIO_AVG, OFF)
- 

## Credit 
This project has taken much inspiration from many different sources. Please see [CONTRIBUTIONS.md](CONTRIBUTIONS.md) for full details. 
