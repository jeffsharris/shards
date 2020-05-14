#include "FastLED.h"

// How many leds in the strip
#define N_LEDS 339

// Size of wave
#define N_WAVE 5

// Delay
#define DELAY 20

#define UPDATE_FREQ 100

// The brightness value to use
#define BRIGHTNESS                  255

#define COLOR_ORDER BRG


// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Data pin that led data will be written out over
#define DATA_PIN 8
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN 6

// Define the array of leds
CRGB leds[N_LEDS];



void setup() { 
      FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, N_LEDS).setCorrection( TypicalLEDStrip ).setTemperature( DirectSunlight);
      FastLED.setBrightness(  BRIGHTNESS );
      //Serial.begin(9600);

}


void waveFill(uint8_t nWaves) {
  // Determine highest bit needed to represent pixel index
  int hiBit = 0;
  int n = N_LEDS - 1;
  for(int bit=1; bit < 0x8000; bit <<= 1) {
    if(n & bit) hiBit = bit;
  }
 for (uint16_t location = 0; location < N_LEDS; location++) {
  int bit, reverse;
  for(int i=0; i<(hiBit << 1); i++) {
    // Reverse the bits in i to create ordered dither:
    reverse = 0;
    for(bit=1; bit <= hiBit; bit <<= 1) {
      reverse <<= 1;
      if(i & bit) reverse |= 1;
    }
    uint8_t val = sin8(map(reverse % (N_LEDS / nWaves), 0, N_LEDS / nWaves, 0, 255));
    leds[((location + reverse) % N_LEDS)] = CHSV(0, 0, val);
      if (random8() < UPDATE_FREQ) {
        FastLED.show();
      }
  }
 }
}


void loop() {
        waveFill(4);
}
