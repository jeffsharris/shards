#include "FastLED.h"

// How many leds in your strip?
#define N_LEDS 274

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Data pin that led data will be written out over
#define DATA_PIN 6
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN 8

// Define the array of leds
CRGB leds[N_LEDS];

void setup() { 

      FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, BRG>(leds, N_LEDS);
}

void loop() { 
  // Turn the LED on, then pause
  uint16_t currentStep;
  for (currentStep = 0; currentStep < N_LEDS; currentStep++) {
    leds[currentStep] = CRGB::White;
    FastLED.show();
    delay(50);
    // Now turn the LED off, then pause
    leds[currentStep] = CRGB::Black;
    FastLED.show();
    delay(50);
  }
}
