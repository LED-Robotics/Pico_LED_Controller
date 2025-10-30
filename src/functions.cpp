#include "functions.h"
#include "commands.h"

int ledLength = 1080;

PicoLed::Color purple{166, 0, 255};

void runCommand(uint16_t command, uint8_t *data) {
  switch(command) {
    case DEBUG_BLINK:
      debugBlink();
      break;
    case LEDS_OFF:
      ledsOff(data);
      break;
    case LEDS_ON:
      ledsOn(data);
      break;
    case IDLE_ANIMATE:
      idleAnimate(data);
      break;
  }
}

void debugBlink() {
  // gpio_put(LED_PIN, true);
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
  sleep_ms(250);
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
}

void ledsOff(uint8_t* data) {

}

void ledsOn(uint8_t* data) {
  
}

void idleAnimate(uint8_t* data) {
  
}

void scrollStrip(PicoLed::PicoLedController* strip, bool flip) {
  PicoLed::Color previous[ledLength];
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

void fillGreenPurple(PicoLed::PicoLedController* strip, int swaps, bool startGreen) {
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
