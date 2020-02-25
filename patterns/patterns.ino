#include "FastLED.h"

// How many leds in the strip
#define N_LEDS 339

// Size of wave
#define N_WAVE 5

// Delay
#define DELAY 50

// The brightness value to use
#define BRIGHTNESS                  255

#define COLOR_ORDER BRG

#define DECAY 128

#define N_WAVES 10

#define MIN_BRIGHT 50
#define MAX_BRIGHT 255

#define MIN_LENGTH 5
#define MAX_LENGTH 20

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Data pin that led data will be written out over
#define DATA_PIN 8
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN 6

// Define the array of leds
CRGB leds[N_LEDS];

struct Wave {
  uint16_t location;
  uint8_t brightness;
  uint8_t travelSpeed;
  uint8_t lifetime;
  int creationTime;
};

struct Wave waves[N_WAVES];

int elapsedTime = 0;

void setup() { 
      FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, N_LEDS).setCorrection( TypicalLEDStrip ).setTemperature( DirectSunlight);
      FastLED.setBrightness(  BRIGHTNESS );
      Serial.begin(9600);
      for (uint8_t i = 0; i < N_WAVES; i++) {
        randomWave(&waves[i]);
      }
}

void randomWave(struct Wave * waveAddress) {
  waveAddress->location = random(N_LEDS);
  waveAddress->brightness = random(MIN_BRIGHT, MAX_BRIGHT);
  waveAddress->travelSpeed = 1;
  waveAddress->lifetime = random(MIN_LENGTH, MAX_LENGTH);
  waveAddress->creationTime = elapsedTime;
}


void loop() {

    fill_solid(leds, N_LEDS, CRGB::Black);
    for (uint8_t i = 0; i < N_WAVES; i++) {
      Wave currentWave = waves[i];
      uint8_t age = elapsedTime - currentWave.creationTime;
      for (uint8_t j = 1; j <= age; j++) {
        uint8_t ledbrightness = leds[(currentWave.location + j) % N_LEDS].red + currentWave.brightness * j / (age * 1.0);
        leds[(currentWave.location + j) % N_LEDS] = CRGB(ledbrightness, ledbrightness, ledbrightness);
        leds[(currentWave.location - j) % N_LEDS] = CRGB(ledbrightness, ledbrightness, ledbrightness);
      }
      if (age == currentWave.lifetime) {
        randomWave(&waves[i]);
      }
    }
    FastLED.show();
    delay(DELAY);
    elapsedTime++;
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

void bloomWave(uint16_t location, uint16_t halfLength, uint8_t brightness) {
  leds[location] = CHSV(0, 0, brightness); 
  FastLED.show();
  delay(DELAY);
  for (uint16_t i = 0; i < halfLength; i++) {
    leds[location].nscale8(DECAY);
    for (uint16_t j = 1; j <= i; j++) {
      if (j < i) {
        leds[(location + j) % N_LEDS].nscale8(DECAY);
        leds[(location - j) % N_LEDS].nscale8(DECAY);
      } else {
        leds[(location + j) % N_LEDS] = CHSV(0, 0, 255);
        leds[(location - j) % N_LEDS] = CHSV(0, 0, 255);
      }
    }
    FastLED.show();
    delay(DELAY);
  }
}

void brightnessWave(uint16_t len, uint16_t space) {
  for (uint16_t i = 0; i < N_LEDS; i++) {
    for (uint16_t location  = 0; location < N_LEDS; location++) {
      if (location % (len + space) < len) {
         leds[(i + location) % N_LEDS] = CHSV(0, 0, cos8(map(location % len, 0, len - 1, 0, 127)));
      } else {
         leds[(i + location) % N_LEDS] = CRGB::Black;
      }
      Serial.println(cos8(map(location % len, 0, len - 1, 0, 127)));
    }
    FastLED.show();
    delay(DELAY);
  }
}

void bloom() {
   // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, N_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    placeWave(beatsin16( i+7, 0, N_LEDS-1 ), 20);
    FastLED.show();
  }
}

void placeWave(uint16_t location, uint16_t waveLength) {
  leds[location % N_LEDS] = CHSV(0, 0, 255);
  for(uint16_t i = 0; i < waveLength / 2; i++) {
    leds[(location + i) % N_LEDS] = CHSV(0, 0, 255 - sin8(map(i, 0, waveLength, 0, 128)));
    leds[(location - i) % N_LEDS] = CHSV(0, 0, 255 - sin8(map(i, 0, waveLength, 0, 128)));
  }
  FastLED.show();
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < N_LEDS; i++) {
    for (uint16_t location  = 0; location < N_LEDS; location++) {
      leds[(i + location) % N_LEDS] = CHSV(0, 0, beat);
    }
    FastLED.show();
  }
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, N_LEDS, 20);
  int pos = beatsin16(13,0,N_LEDS);
  leds[pos] += CHSV( 0, 0, 192);
  FastLED.show();
  delay(DELAY);
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
