#include "FastLED.h"

// How many leds in the strip
#define N_LEDS 339

// Size of wave
#define N_WAVE 80

// Delay
#define DELAY 150

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
}

void loop() { 
  // Turn the LED on, then pause
  candyCane(5, 20);
}

void candyCane(uint16_t len, uint16_t space) {
  for (uint16_t i = 0; i < N_LEDS; i++) {
    uint16_t location = 0;
    for (uint16_t location  = 0; location < N_LEDS; location++) {
      if (location % (len + space) < len) {
        leds[(i + location) % N_LEDS] = CRGB::White;
      } else {
        leds[(i + location) % N_LEDS] = CRGB::Black;
      }
    }
    FastLED.show();
    delay(DELAY);
  }
}

void movingSnake(uint8_t len, uint8_t space) {
  uint16_t waveStep;
  for (waveStep = 0; waveStep < N_WAVE; waveStep++) {
    leds[waveStep] = CRGB::White;
  }
  uint16_t currentStep;
  for (currentStep = 0; currentStep < N_LEDS; currentStep++) {
    leds[(currentStep - 1) % N_LEDS] = CRGB::Black;
    leds[(currentStep + N_WAVE) % N_LEDS] = CRGB::White;
    FastLED.show();
    delay(DELAY);
  }
}
