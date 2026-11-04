#ifndef ANIMATOR_H
#define ANIMATOR_H

#include<FastLED.h>

class Animator {
private:
  CRGB* leds;
  int numLeds;

  unsigned long previousMillis = 0;
  int currentSection = 0;
  bool isOn = false;
  int index = 0;
  int fade = 255;

public:
  Animator(CRGB* lights, int length) {
    leds = lights;
    numLeds = length;
  }

  void strobe(unsigned long delay, uint8_t hue = 0, uint8_t sat = 255, uint8_t brightness = 255) {
    int sectionSize = numLeds / 5;
    if (millis() - previousMillis < delay)
      return;
    previousMillis = millis();
    isOn = !isOn;

    if (isOn) {
      currentSection = random(5);
      int start = currentSection * sectionSize;
      int end = start + sectionSize;

      for (int i = start; i < end; i++) {
        if (i < numLeds) {
          leds[i] = CHSV(hue, sat, brightness);
        }
      }
    } else {
      int start = currentSection * sectionSize;
      int end = start + sectionSize;

      for (int i = start; i < end; i++) {
        if (i < numLeds) {
          leds[i] = CRGB::Black;
        }
      }
    }
  }

  void solidStrobe(unsigned long delay, uint8_t hue=0, uint8_t sat=255) {
    if ((millis() - previousMillis) < delay)
      return;
    previousMillis = millis();

    // Dips into negatives in order to provide a moment of darkness
    if (fade > -10) {
      // Ensures LEDs don't receive a negative brightness
      if (fade >= 0) {
        fill_solid(leds, numLeds, CHSV(hue, sat, fade));
      } else {
        FastLED.clear();
      }
      // This is your fade decrement
      fade -= 5;
    } else {
      // Start fade from the top
      fade = 255;
    }
  }

  void fastScroll(unsigned long delay, uint8_t hue = 0, uint8_t sat = 255) {
    // Throttle the frame rate
    if (millis() - previousMillis < delay) return;
    previousMillis = millis();

    const int segmentSize = numLeds / 4;   // four equal chunks
    // Fade timer (255 → 0). When it hits zero, advance to next segment.
    if (fade <= 0) {
      fade = 255;
      currentSection++;
      if (currentSection >= 4) currentSection = 0;
    } else {
      fade -= 15;                // tweak step size to taste
    }

    FastLED.clear();
    // Segment currently lit (fading OUT)
    fill_solid(&leds[currentSection * segmentSize], segmentSize, CHSV(hue, sat, fade));

    // Segment that’s about to light up (fading IN)
    int nextSegment = currentSection + 1;
    if (nextSegment >= 4) nextSegment = 0;

    fill_solid(&leds[nextSegment * segmentSize], segmentSize, CHSV(hue, sat, 255 - fade));
  }

  void smoothScroll(unsigned long delay, uint8_t hue = 0, uint8_t sat = 255, uint8_t bri = 255) {
    if ((millis() - previousMillis) < delay)
      return;
    previousMillis = millis();
    
    //  Reset index to 0 if cycle completes
    if (index > numLeds * 2) {
      index = 0;
    }

    if (index < numLeds) {
      //  FILL
      leds[index] = CHSV(hue, sat, bri);
    } else {
      //  EMPTY
      leds[index - numLeds] = CRGB::Black;
    }
    index++;
  }

  void reset() {
    previousMillis = 0;
    currentSection = 0;
    index = 0;
    isOn = false;
    fade = 255;

    FastLED.clear();
  }
};

#endif // ANIMATOR_H