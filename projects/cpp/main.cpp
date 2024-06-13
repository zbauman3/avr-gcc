#include <SNX4HC595.c>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <helpers.c>
#include <pins.c>
#include <util/delay.h>

SNX4HC595Config config = {.clrb = 13, .sclk = 9, .rclk = 11, .data = 8};

volatile bool didInterrupt = false;
static uint8_t save = 0b00000001;
static uint8_t counter = 0b00000001;

uint16_t leftRotate(uint16_t n, uint16_t d) {
  return (n << d) | (n >> (16 - d));
}

void loop() {
  if (didInterrupt == true) {
    didInterrupt = false;
    save = counter;

    SNX4HC595_sendWord(&config, 0b1111111111111111);
    _delay_ms(1000);
  }

  uint16_t word = leftRotate((uint16_t)counter, 8) | save;

  SNX4HC595_sendWord(&config, word);

  counter *= 2;
  if (counter == 0) {
    counter = 1;
  }

  _delay_ms(500);
}

int main(void) {
  SNX4HC595_setup(&config);
  SNX4HC595_clear(&config);
  pinMode(6, INPUT);

  sbi(GIMSK, PCIE0);
  sbi(PCMSK0, PCINT7);

  sei();

  for (;;)
    loop();

  return (0);
}

ISR(PCINT0_vect) { didInterrupt = true; }
