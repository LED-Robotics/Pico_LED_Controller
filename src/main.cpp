#include "pico/stdlib.h"
#include <cstdint>
#include <stdio.h>

#include <Effects/Bounce.hpp>
#include <Effects/Comet.hpp>
#include <Effects/Marquee.hpp>
#include <Effects/Particles.hpp>
#include <Effects/Stars.hpp>
#include <PicoLed.hpp>

#include "functions.h"

#include "hardware/uart.h"

#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define UART_ID uart1
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define PACKET_SIZE 50

#define LED_PIN 0

uint8_t rxBuf[PACKET_SIZE];

int main() {
  stdio_init_all();

  gpio_set_function(0, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
  gpio_set_function(1, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

  // Initialise UART
  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
  uart_init(UART_ID, BAUD_RATE);

  // uart_puts(UART_ID, "Hello world!");

  sleep_ms(1000);

  // 0. Initialize LED strip
  // auto mainStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN,
  // ledLength, PicoLed::FORMAT_GRB); mainStrip.setBrightness(60);
  //
  // mainStrip.clear();
  // mainStrip.show();

  while (true) {
    if (uart_is_readable(UART_ID)) {
      uart_read_blocking(UART_ID, &rxBuf[0], PACKET_SIZE);
      uint16_t id = 0;
      for(int i = 0; i < 4; i++) {
        id += rxBuf[i] << ((4 - i) * 4);
      }
      runCommand(id, &rxBuf[4]);
    }
    sleep_ms(1);
  }
  return 0;
}
