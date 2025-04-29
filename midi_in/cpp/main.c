#include "bsp/board_api.h"
#include "hardware/gpio.h"
#include "tusb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define GPIO pins for clock and run signals
#define CLOCK_OUT_PIN 2
#define RUN_OUT_PIN 3

void led_blinking_task(void);
void midi_task(void);

void pulse_gpio(uint gpio);
void update_led_state(void);

volatile bool clock_active = false;
uint32_t last_clock_time = 0;

void pulse_gpio(uint gpio) {
  gpio_put(gpio, 1);
  sleep_us(500); // Short pulse
  gpio_put(gpio, 0);
}

int main(void) {
  board_init();
  tud_init(BOARD_TUD_RHPORT);

  // Initialize GPIOs
  gpio_init(CLOCK_OUT_PIN);
  gpio_set_dir(CLOCK_OUT_PIN, GPIO_OUT);
  gpio_put(CLOCK_OUT_PIN, 0);

  gpio_init(RUN_OUT_PIN);
  gpio_set_dir(RUN_OUT_PIN, GPIO_OUT);
  gpio_put(RUN_OUT_PIN, 0);

  while (1) {
    tud_task();
    midi_task();
    led_blinking_task();
  }
}

// MIDI Task
void midi_task(void) {
  uint8_t packet[4];

  // ensure full packet is available
  while (tud_midi_available() >= 4) {
    // ensure we got a full MIDI message
    uint32_t count = tud_midi_stream_read(packet, sizeof(packet));
    if (count < 4)
      return;

    uint8_t status = packet[0];

    if (status == 0xF8) { // MIDI Clock
      printf("MIDI Clock received\n");
      pulse_gpio(CLOCK_OUT_PIN);
      clock_active = true;
      last_clock_time = board_millis();
    } else if (status == 0xFA) { // Start
      printf("MIDI Start received\n");
      gpio_put(RUN_OUT_PIN, 1);
    } else if (status == 0xFC) { // Stop
      printf("MIDI Stop received\n");
      gpio_put(RUN_OUT_PIN, 0);
    }
  }
}

// LED BLINKING TASK - Only blink if clock is active
void led_blinking_task(void) {
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // if no MIDI clock received for 500ms, turn off LED
  if (board_millis() - last_clock_time > 500) {
    clock_active = false;
    board_led_write(0);
    return;
  }

  if (clock_active && (board_millis() - start_ms >= 250)) {
    start_ms += 250;
    led_state = !led_state;
    board_led_write(led_state);
  }
}
