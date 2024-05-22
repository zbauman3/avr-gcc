#include <avr/io.h>
#include <helpers.h>
#include <util/delay.h>

uint8_t last = 1;

void loop() {
  digitalToggle(PORTB, PINB0);
  _delay_ms(1000);
}

int main(void) {
  pinMode(DDRB, PINB0, OUTPUT);

  for (;;)
    loop();

  return (0);
}