#pragma once

#include "PicoLedController.hpp"
#include "pico/stdlib.h"
#include <cstdint>
#include <functional>

/*
* FIRST TWO BYTES OF EVERY PACKET ARE THE COMMAND ID
* PAYLOAD FOLLOWS IMMEDIATELY AFTER
*/

struct ledStrip {
  uint8_t id;
  uint8_t stateMachine;
  PicoLed::PicoLedController strip;
  bool isVirtual = false;
  std::function<void()> animate = nullptr;
};

struct Request {
  uint16_t commandId;
  uint8_t* payload;
  uint16_t len;
};

#define DEBUG_BLINK 0x69fa

#define LEDS_OFF 0x0eaf
struct LedsOffRequest {
  uint8_t stripId;
};

#define IDLE_ANIMATE 0xa146
struct IdleAnimateRequest {
  uint8_t stripId;
  uint8_t swaps;
  uint8_t startGreen;
};

#define FILL_STRIP 0xc273
struct FillStripRequest {
  uint8_t stripId;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

#define CREATE_STRIP 0x8bda
struct CreateStripRequest {
  uint8_t stripPin;
  uint16_t ledLength;
};

#define DESTROY_STRIP 0x10e7
struct DestroyStripRequest {
  uint8_t stripId;
};

#define CREATE_SLICE 0x3138
struct CreateSliceRequest {
  uint8_t stripId;
  uint16_t startLed;
  uint16_t endLed;
};

