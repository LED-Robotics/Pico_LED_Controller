#include "functions.h"
#include "PicoLed.hpp"
#include "PicoLedController.hpp"
#include "commands.h"
#include <cstdint>
#include <pico.h>

int ledLength = 1080;

PicoLed::Color purple{166, 0, 255};

std::vector<uint8_t> usedStateMachines;
std::vector<ledStrip> strips;


// Run command given ID, pass on data payload where relevant
void runCommand(uint16_t command, uint8_t *data) {
  switch(command) {
    case DEBUG_BLINK:
      debugBlink();
      break;
    case LEDS_OFF:
      ledsOff(data);
      break;
    case IDLE_ANIMATE:
      idleAnimate(data);
      break;
    case FILL_STRIP:
      fillStrip(data);
      break;
    case CREATE_STRIP:
      createStrip(data);
      break;
    case CREATE_SLICE:
      createSlice(data);
      break;
    case DESTROY_STRIP:
      destroyStrip(data);
      break;
  }
}

// Blink Pico onboard LED
void debugBlink() {
  // gpio_put(LED_PIN, true);
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
  sleep_ms(250);
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
}

// Clear LED strip
void ledsOff(uint8_t* data) {
  LedsOff* payload = (LedsOff*)data;
  auto strip = getStripFromId(payload->stripId);
  strip->strip.clear();
}

// Set strip to idle animate
void idleAnimate(uint8_t* data) {
  IdleAnimate* payload = (IdleAnimate*)data;
  ledStrip* strip = getStripFromId(payload->stripId);
  fillGreenPurple(&strip->strip, payload->swaps, payload->startGreen);
  strip->animate = [payload]() { scrollStrip(payload->stripId); };
}

// Fill strip with color
void fillStrip(uint8_t* data) {
  FillStrip* payload = (FillStrip*)data;
  ledStrip* strip = getStripFromId(payload->stripId);
  strip->strip.fill({payload->red, payload->green, payload->blue});
  strip->animate = [payload]() { scrollStrip(payload->stripId); };
}

// Create LED strip and add to relevant vectors
void createStrip(uint8_t* data) {
  CreateStrip* payload = (CreateStrip*)data;
  uint8_t stateMachine = getAvailableStateMachine();
  PIO bank = getAvailablePioBank(stateMachine);
  if(stateMachine > 3) stateMachine -= 4;

  uint8_t* high = &data[1];
  uint8_t* low = &data[2];
  payload->ledLength = (*high << 8) + *low;

  ledStrip strip{
    getNewId(),
    stateMachine,
    PicoLed::addLeds<PicoLed::WS2812B>(bank, stateMachine, payload->stripPin, payload->ledLength, PicoLed::FORMAT_GRB),
    false
  };
  strip.strip.setBrightness(60);
  usedStateMachines.push_back(stateMachine);
  strips.push_back(strip);
}

void destroyStrip(uint8_t *data) {
  DestroyStrip* payload = (DestroyStrip*)data;
  auto strip = getStripFromId(payload->stripId);

  for(int i = 0; i < strips.size(); i++) {
    if(strips[i].id == payload->stripId) {
      strips.erase(strips.begin()+i);
    }
  }

}

//  Create virtual LED strip
void createSlice(uint8_t* data) {
  CreateSlice* payload = (CreateSlice*)data;
  auto original = getStripFromId(payload->stripId);
  ledStrip strip{
    getNewId(),
    original->stateMachine,
    original->strip.slice(payload->startLed, payload->endLed),
    true
  };
  strips.push_back(strip);
}

// Get unused ID for new strip
uint8_t getNewId() {
  for(int i = 0; i < 0xff; i++) {
    bool matched = false;
    for(auto &strip : strips) {
      if(strip.id == i) matched = true;
    }
    if(!matched) return i;
  }
  return 69;
}

// Get available PIO bank
PIO getAvailablePioBank(uint8_t stateMachine) {
  if(stateMachine < 4) return pio0;
  else return pio1;
}

// Get available State Machine
uint8_t getAvailableStateMachine() {
  for(int i = 0; i < 7; i++) {
    if(count(usedStateMachines.begin(), usedStateMachines.end(), i) == 0)
      return i;
  }
  return 0;
}

// Find LED strip given an ID
ledStrip* getStripFromId(uint8_t id) {
  for(auto &strip : strips) {
    if(strip.id == id) return &strip;
  }
  return &strips[0];
}

// Animate strip moving in a direction
void scrollStrip(uint8_t id, bool flip) {
  auto strip = getStripFromId(id)->strip;
  PicoLed::Color previous[ledLength];
  auto num = strip.getNumLeds();
  for(int i = 0; i < num; i++) {
    previous[i] = strip.getPixelColor(i);
  }
  if(flip) {
    for(int i = num - 1; i > 0; i--) {
      if(i - 1 < 0) {
        // Wrap to end
        strip.setPixelColor(num - 1, previous[i]);
      } else {
        if(i + 1 >= num - 1) {
          // Wrap to beginning
          strip.setPixelColor(i, previous[0]);
        } else {
          // Move pixel back
          strip.setPixelColor(i, previous[i + 1]);
        }
        // Move next pixel back
        strip.setPixelColor(i - 1, previous[i]);
      }
    }
    return;
  }
  for(int i = 0; i < num; i++) {
    if(i + 1 >= num) {
      // Wrap to beginning
      strip.setPixelColor(0 + i - num, previous[i]);
    } else {
      if(i - 1 < 0) {
        // Wrap to end
        strip.setPixelColor(i, previous[num + (i - 1)]);
      } else {
        // Move pixel forward
        strip.setPixelColor(i, previous[i - 1]);
      }
      // Move previous pixel forward
      strip.setPixelColor(i + 1, previous[i]);
    }
  }
}

// Fill strip with green and purple subdivisions
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

