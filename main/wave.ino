#include <FastLED.h>
#include "GPLC_Pins.h"

#define NUM_LEDS 120
#define DATA_PIN 5
#define LED_TYPE WS2812B

#define SPEEDCREASE 2

float i = 0;
float speed_ = 1;

void updateLEDs(CRGB* leds) {
  leds[0] = getWaveAtIndex(i);  
  shiftAll(leds);

  if (i+speed_ <= 255) {
    i+=speed_;
  }
  else {
    i = 0;
  }
}

CRGB getWaveAtIndex(float i) {
  float t = i / 255;
  float w = lerp(380, 840, i/255);
  float R = 0.0;
  float G = 0.0;
  float B = 0.0;

  if (w >= 380 and w < 440){
        R = -(w - 440.) / (440. - 380.);
        G = 0.0;
        B = 1.0;
  }
  else if (w >= 440 and w < 490){
      R = 0.0;
      G = (w - 440.) / (490. - 440.);
      B = 1.0;
  }
  else if (w >= 490 and w < 510){
      R = 0.0;
      G = 1.0;
      B = -(w - 510.) / (510. - 490.);
  }
  else if (w >= 510 and w < 580){
      R = (w - 510.) / (580. - 510.);
      G = 1.0;
      B = 0.0;
  }
  else if (w >= 580 and w < 645){
      R = 1.0;
      G = -(w - 645.) / (645. - 580.);
      B = 0.0;
  }
  else if (w >= 645 and w <= 780){
      R = 1.0;
      G = 0.0;
      B = 0.0;
  }
  else if (w >= 780 and w <= 840){
      R = 1.0;
      G = 0.0;
      B = (w - 780.) / (840. - 780.);
  }
  else{
      R = 0.0;
      G = 0.0;
      B = 0.0;
  }
  
  return CRGB((R*255), (G*255), (B*255));
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
