#include <stdio.h>
#include "pico/stdlib.h"

#include <PicoLed.hpp>
#include <Effects/Marquee.hpp>
#include <Effects/Stars.hpp>
#include <Effects/Comet.hpp>
#include <Effects/Bounce.hpp>
#include <Effects/Particles.hpp>

#define LED_PIN 0
#define LED_LENGTH 1080

PicoLed::Color previous[LED_LENGTH];

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
  bool fillGreen = true;
  for(int i = 0; i < LED_LENGTH; i += (LED_LENGTH / swaps)) {
    if(fillGreen) {
      strip->fill({0, 255, 0}, i, LED_LENGTH / swaps);
    }
    else {
      strip->fill({102, 0, 255}, i, LED_LENGTH / swaps);
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

  /*bool fillGreen = true;*/
  /*int swaps = 30;*/
  /*for(int i = 0; i < LED_LENGTH; i += (LED_LENGTH / swaps)) {*/
  /*  if(fillGreen) {*/
  /*    mainStrip.fill({0, 255, 0}, i, LED_LENGTH / swaps);*/
  /*  }*/
  /*  else {*/
  /*    mainStrip.fill({102, 0, 255}, i, LED_LENGTH / swaps);*/
  /*  }*/
  /*  fillGreen = !fillGreen;*/
  /*}*/

  // CASCADE CHANNEL 1 starts at 250
  // CASCADE CHANNEL 1 ends at 500
  /*mainStrip.setPixelColor(372, {255, 255, 255});*/
  auto leftCascade1 = mainStrip.slice(250, 370);
  auto leftCascade2 = mainStrip.slice(380, 500);

  auto middle = mainStrip.slice(501, 578);

  auto rightCascade1 = mainStrip.slice(579, 699);
  auto rightCascade2 = mainStrip.slice(709, 829);

  auto beginning = mainStrip.slice(0, 249);
  auto end = mainStrip.slice(830, LED_LENGTH - 1);

  PicoLed::Comet beginningEffect(beginning, {102, 0, 255}, 80.0, 40.0, 20.0);

  PicoLed::Comet middleEffect(middle, {0, 255, 0}, 80.0, 20.0, 20.0);

  PicoLed::Comet endEffect(end, {0, 255, 0}, 80.0, 40.0, 20.0);

  /*rightCascade1.setPixelColor(0, {255, 255, 255});*/
  /*rightCascade2.setPixelColor(0, {255, 255, 255});*/
  /*rightCascade1.setPixelColor(rightCascade1.getNumLeds(), {255, 255, 255});*/
  /*rightCascade2.setPixelColor(rightCascade2.getNumLeds(), {255, 255, 255});*/

  fillGreenPurple(&leftCascade1, 30, false);
  fillGreenPurple(&leftCascade2, 30);

  fillGreenPurple(&rightCascade1, 30, false);
  fillGreenPurple(&rightCascade2, 30);

  mainStrip.show();

  /*mainStrip.fill({0, 255, 4}, 0, LED_LENGTH / 2);*/
  /*mainStrip.fill({102, 0, 255}, LED_LENGTH / 2, LED_LENGTH / 2 - 1);*/
  /*mainStrip.show();*/

  while(true) {
      
      /*scrollStrip(&mainStrip);*/
      scrollStrip(&leftCascade1);
      scrollStrip(&leftCascade2, true);
      scrollStrip(&rightCascade1, true);
      scrollStrip(&rightCascade2);
      beginningEffect.animate();
      middleEffect.animate();
      endEffect.animate();
      mainStrip.show();
      sleep_ms(1);
    }
    return 0;
}
