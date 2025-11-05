#pragma once

#include <cstdint>
#include <hardware/pio.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include <PicoLed.hpp>
#include "commands.h"
#include <vector>

extern int ledLength;

extern PicoLed::Color purple;

extern std::vector<uint8_t> usedStateMachines;
extern std::vector<ledStrip> strips;

// Helper functions
// void scrollStrip(PicoLed::PicoLedController* strip, bool flip = false);
void scrollStrip(uint8_t id, bool flip = false);
void fillGreenPurple(PicoLed::PicoLedController* strip, int swaps, bool startGreen = true);
uint8_t getNewId();
PIO getAvailablePioBank(uint8_t stateMachine);
uint8_t getAvailableStateMachine();
ledStrip* getStripFromId(uint8_t id);

// Commands from registry
void runCommand(uint16_t command, uint8_t *data);

void debugBlink();
void ledsOff(uint8_t* data);
void idleAnimate(uint8_t* data);
void fillStrip(uint8_t* data);
void createStrip(uint8_t* data);
void destroyStrip(uint8_t* data);
void createSlice(uint8_t* data);
