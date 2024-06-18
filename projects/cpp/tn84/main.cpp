#include <SNX4HC595.c>
#include <avr/io.h>
#include <util/delay.h>

SNX4HC595Config config = {.clrb = 13, .sclk = 9, .rclk = 11, .data = 8};

uint16_t state = 0;

void loop() {
  state++;
  if (state == 0) {
    state = 1;
  }

  SNX4HC595_sendWord(&config, state);
  _delay_ms(250);
}

int main(void) {
  SNX4HC595_setup(&config);
  SNX4HC595_clear(&config);

  for (;;)
    loop();

  return (0);
}
