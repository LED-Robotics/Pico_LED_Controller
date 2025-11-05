#include "pico/stdlib.h"
#include <boards/pico2_w.h>
#include "pico/cyw43_arch.h"
#include <cstdint>
#include <hardware/structs/io_bank0.h>
#include <stdio.h>

#include <Effects/Bounce.hpp>
#include <Effects/Comet.hpp>
#include <Effects/Marquee.hpp>
#include <Effects/Particles.hpp>
#include <Effects/Stars.hpp>
#include <PicoLed.hpp>

#include "functions.h"

#include "hardware/uart.h"

#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define PACKET_SIZE 50

uint8_t* rxBuf;

int main() {
  stdio_init_all();
  cyw43_arch_init();

  rxBuf = (uint8_t*)malloc(PACKET_SIZE);

  // gpio_init(LED_PIN);
  // gpio_set_dir(LED_PIN, GPIO_OUT);

  // Initialise UART
  uart_init(UART_ID, BAUD_RATE);
  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  sleep_ms(1000);

  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
  sleep_ms(250);
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);

  printf("Debug Init\n");

  while (true) {
    // UART command handling
    if (uart_is_readable(UART_ID)) {
      // Clear buffer
      memset(rxBuf, 0x00, PACKET_SIZE);
      // Fill buffer with available bytes
      for(int i = 0; i < PACKET_SIZE; i++) {
        if(!uart_is_readable(UART_ID)) break;
        rxBuf[i] = uart_getc(UART_ID);
        printf("%x", (int)rxBuf[i]);
      }
      printf("\n");
      // Assemble packet ID
      uint8_t* high = &rxBuf[0];
      uint8_t* low = &rxBuf[1];
      uint16_t id = (*high << 8) + *low;
      printf("Received ID: %d\n", id);
      // Run command associated with ID
      runCommand(id, &rxBuf[2]);
    }
    //  Animation handling
    for(auto &strip : strips) {
      if(strip.animate != nullptr) {
        // Run animation commands
        strip.animate();
      }
    }
    for(auto &strip : strips) {
      if(!strip.isVirtual) {
        // Flush changes to actual hardware controllers
        strip.strip.show();
      }
    }
    sleep_ms(20);
  }
  return 0;
}
