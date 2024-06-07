#include <SNX4HC595.c>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

SNX4HC595Config config = {.clrb = 13, .sclk = 9, .rclk = 11, .data = 8};

void loop() {
  static uint16_t last = 0b1111111100000000;

  last = ~last;

  SNX4HC595_sendWord(&config, last);

  _delay_ms(1000);
}

int main(void) {
  SNX4HC595_setup(&config);
  SNX4HC595_clear(&config);

  for (;;)
    loop();

  return (0);
}
