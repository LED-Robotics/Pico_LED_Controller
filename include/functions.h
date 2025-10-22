#include <cstdint>
#include <stdio.h>
#include "pico/stdlib.h"

#include <PicoLed.hpp>

int ledLength = 1080;

PicoLed::Color purple{166, 0, 255};

// Helper functions

void scrollStrip(PicoLed::PicoLedController* strip, bool flip = false);
void fillGreenPurple(PicoLed::PicoLedController* strip, int swaps, bool startGreen = true);

// Commands from registry
void runCommand(uint16_t command, uint8_t *data);

void ledsOff(uint8_t* data);
void ledsOn(uint8_t* data);
void idleAnimate(uint8_t* data);
