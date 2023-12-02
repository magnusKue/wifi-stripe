#include <FastLED.h>
#include "GPLC_Pins.h"

#define NUM_LEDS 120
#define DATA_PIN 5
#define LED_TYPE WS2812B

#define SPEEDCREASE 2

float i = 0;
float speed_ = 3;

void updateLEDs(CRGB* leds) {
  int cycleDuration = 5000;
  float normalizedValue = float(millis() % cycleDuration) / float(cycleDuration);
  int hue = normalizedValue * 255;
  leds[0] = CHSV(hue, 255, 255);  
  shiftAll(leds);
}

void shiftAll(CRGB* leds) {
  for (int x = NUM_LEDS; x >= 1; x--) {
    leds[x] = leds[x-1];
  }
}

int clamp(int x, int minV, int maxV) {
  return max(min(x, maxV), minV);
}

float lerp(float a, float b, float t)
{
    return a * (1.0 - t) + (b * t);
}
