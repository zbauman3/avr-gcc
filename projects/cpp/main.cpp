#include <SNX4HC595.c>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

SNX4HC595Config config = {.clrb = 13, .sclk = 9, .rclk = 11, .data = 8};

uint16_t numbToBitMask(uint8_t num) { return (1 << num); };

void loop() {
  static uint16_t last = 0b0000000000000001;

  SNX4HC595_sendWord(&config, last);

  last *= 2;
  if (last == 0) {
    last = 1;
  }

  _delay_ms(500);
}

int main(void) {
  SNX4HC595_setup(&config);
  SNX4HC595_clear(&config);

  for (;;)
    loop();

  return (0);
}
