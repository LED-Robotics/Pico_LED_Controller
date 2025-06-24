#include <stdio.h>
#include <cstdint>
#include "pico/stdlib.h"

#include <PicoLed.hpp>
#include <Effects/Marquee.hpp>
#include <Effects/Stars.hpp>
#include <Effects/Comet.hpp>
#include <Effects/Bounce.hpp>
#include <Effects/Particles.hpp>

#define LED_PIN 0
#define LED_LENGTH 540

PicoLed::Color previous[LED_LENGTH];

PicoLed::Color purple{166, 0, 255};

void scrollStrip(PicoLed::PicoLedController* strip, bool flip = false) {
  PicoLed::Color previous[LED_LENGTH];
  auto num = strip->getNumLeds();
  for(int i = 0; i < num; i++) {
    previous[i] = strip->getPixelColor(i);
  }
  if(flip) {
    for(int i = num - 1; i > 0; i--) {
      if(i - 1 < 0) {
        // Wrap to end
        strip->setPixelColor(num - 1, previous[i]);
      } else {
        if(i + 1 >= num - 1) {
          // Wrap to beginning
          strip->setPixelColor(i, previous[0]);
        } else {
          // Move pixel back
          strip->setPixelColor(i, previous[i + 1]);
        }
        // Move next pixel back
        strip->setPixelColor(i - 1, previous[i]);
      }
    }
    return;
  }
  for(int i = 0; i < num; i++) {
    if(i + 1 >= num) {
      // Wrap to beginning
      strip->setPixelColor(0 + i - num, previous[i]);
    } else {
      if(i - 1 < 0) {
        // Wrap to end
        strip->setPixelColor(i, previous[num + (i - 1)]);
      } else {
        // Move pixel forward
        strip->setPixelColor(i, previous[i - 1]);
      }
      // Move previous pixel forward
      strip->setPixelColor(i + 1, previous[i]);
    }
  }
}

void fillGreenPurple(PicoLed::PicoLedController* strip, int swaps, bool startGreen = true) {
  bool fillGreen = startGreen;
  int length = strip->getNumLeds();
  for(int i = 0; i < length; i += (length / swaps)) {
    if(fillGreen) {
      strip->fill({0, 255, 0}, i, length / swaps);
    }
    else {
      strip->fill(purple, i, length / swaps);
    }
    fillGreen = !fillGreen;
  }
}

int main()
{
  stdio_init_all();
  sleep_ms(1000);

  // 0. Initialize LED strip
  auto mainStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);
  mainStrip.setBrightness(127);

  mainStrip.clear();
  mainStrip.show();

  fillGreenPurple(&mainStrip, 6);

  mainStrip.show();

  while(true) {
      
      scrollStrip(&mainStrip);

      mainStrip.show();
      sleep_ms(1);
    }
    return 0;
}
