#include <stdio.h>
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

void scrollStrip(PicoLed::PicoLedController* strip) {
  auto num = strip->getNumLeds();
  for(int i = 0; i < num; i++) {
    previous[i] = strip->getPixelColor(i);
  }
  for(int i = 0; i < num; i++) {
    if(i + 1 >= num) {
      strip->setPixelColor(0 + i - num, previous[i]);
    } else {
      if(i - 1 < 0) {
        strip->setPixelColor(i, previous[num + (i - 1)]);
      } else {
        strip->setPixelColor(i, previous[i - 1]);
      }
      strip->setPixelColor(i + 1, previous[i]);
    }
  }
  strip->show();
}

int main()
{
  stdio_init_all();
  sleep_ms(1000);

  // 0. Initialize LED strip
  printf("0. Initialize LED strip\n");
  auto mainStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);
  mainStrip.setBrightness(127);

  mainStrip.clear();
  mainStrip.fill({0, 255, 4}, 0, LED_LENGTH / 2 - 1);
  mainStrip.fill({102, 0, 255}, LED_LENGTH / 2, LED_LENGTH / 2);

  mainStrip.show();

  /*auto sub1 = mainStrip.slice(0, 50);*/
  /*auto sub2 = mainStrip.slice(51, 101);*/
  /*auto sub3 = mainStrip.slice(102, 152);*/

  /*sub1.clear(); */
  /*sub2.clear(); */
  /*sub3.clear(); */
  /*sub1.fill({255, 0, 0}, 0, 20); */
  /*sub2.fill({0, 0, 255}, 0, 20); */
  /*sub3.fill({0, 255, 0}, 0, 20); */

  /*sub1.show();*/
  /*sub2.show();*/
  /*sub3.show();*/

  while(true) {
      
      /*scrollStrip(&sub1);*/
      /*sleep_ms(5);*/
      /*scrollStrip(&sub2);*/
      /*sleep_ms(5);*/
      /*scrollStrip(&sub3);*/
      /*sleep_ms(5);*/
      scrollStrip(&mainStrip);
      sleep_ms(1);
    }
    return 0;
}
